//
//  token/glyph_stroker.cc
//
//  The MIT License
//
//  Copyright (C) 2015-2016 Shota Matsuda
//
//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
//  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.
//

#include "token/glyph_stroker.h"

#include <cassert>
#include <cstddef>
#include <iostream>
#include <iterator>
#include <unordered_map>
#include <utility>
#include <vector>

#include "SkPaint.h"
#include "SkPath.h"
#include "SkPathOps.h"

#include "takram/graphics.h"
#include "token/glyph_outline.h"
#include "token/ufo/glyph.h"

namespace token {

namespace {

inline SkPaint::Cap convertCap(GlyphStroker::Cap cap) {
  switch (cap) {
    case GlyphStroker::Cap::UNDEFINED:
      return SkPaint::Cap::kDefault_Cap;
    case GlyphStroker::Cap::BUTT:
      return SkPaint::Cap::kButt_Cap;
    case GlyphStroker::Cap::ROUND:
      return SkPaint::Cap::kRound_Cap;
    case GlyphStroker::Cap::PROJECT:
      return SkPaint::Cap::kSquare_Cap;
    default:
      assert(false);
      break;
  }
  return SkPaint::Cap::kDefault_Cap;
}

inline SkPaint::Join convertJoin(GlyphStroker::Join join) {
  switch (join) {
    case GlyphStroker::Join::UNDEFINED:
      return SkPaint::Join::kDefault_Join;
    case GlyphStroker::Join::MITER:
      return SkPaint::Join::kMiter_Join;
    case GlyphStroker::Join::ROUND:
      return SkPaint::Join::kRound_Join;
    case GlyphStroker::Join::BEVEL:
      return SkPaint::Join::kBevel_Join;
    default:
      assert(false);
      break;
  }
  return SkPaint::Join::kDefault_Join;
}

inline takram::Shape2d convertShape(const SkPath& other) {
  takram::Shape2d shape;
  SkPath::RawIter itr(other);
  SkPath::Verb verb;
  std::vector<SkPoint> points(4);
  while ((verb = itr.next(points.data())) != SkPath::kDone_Verb) {
    switch (verb) {
      case SkPath::Verb::kMove_Verb:
        shape.moveTo(points[0].x(), points[0].y());
        break;
      case SkPath::Verb::kLine_Verb:
        shape.lineTo(points[1].x(), points[1].y());
        break;
      case SkPath::Verb::kQuad_Verb:
        shape.quadraticTo(points[1].x(), points[1].y(),
                          points[2].x(), points[2].y());
        break;
      case SkPath::Verb::kConic_Verb:
        shape.conicTo(points[1].x(), points[1].y(),
                      points[2].x(), points[2].y(),
                      itr.conicWeight());
        break;
      case SkPath::Verb::kCubic_Verb:
        shape.cubicTo(points[1].x(), points[1].y(),
                      points[2].x(), points[2].y(),
                      points[3].x(), points[3].y());
        break;
      case SkPath::Verb::kClose_Verb:
        shape.close();
        break;
      default:
        assert(false);
        break;
    }
  }
  return std::move(shape);
}

inline SkPath convertShape(const takram::Shape2d& other) {
  SkPath path;
  for (const auto& command : other) {
    switch (command.type()) {
      case takram::graphics::CommandType::MOVE:
        path.moveTo(command.point().x, command.point().y);
        break;
      case takram::graphics::CommandType::LINE:
        path.lineTo(command.point().x, command.point().y);
        break;
      case takram::graphics::CommandType::QUADRATIC:
        path.quadTo(command.control().x, command.control().y,
                    command.point().x, command.point().y);
        break;
      case takram::graphics::CommandType::CONIC:
        path.conicTo(command.control().x, command.control().y,
                     command.point().x, command.point().y,
                     command.weight());
        break;
      case takram::graphics::CommandType::CUBIC:
        path.cubicTo(command.control1().x, command.control1().y,
                     command.control2().x, command.control2().y,
                     command.point().x, command.point().y);
        break;
      case takram::graphics::CommandType::CLOSE:
        path.close();
        break;
      default:
        assert(false);
        break;
    }
  }
  return std::move(path);
}

inline SkPath convertPath(const takram::Path2d& other) {
  SkPath path;
  for (const auto& command : other) {
    switch (command.type()) {
      case takram::graphics::CommandType::MOVE:
        path.moveTo(command.point().x, command.point().y);
        break;
      case takram::graphics::CommandType::LINE:
        path.lineTo(command.point().x, command.point().y);
        break;
      case takram::graphics::CommandType::QUADRATIC:
        path.quadTo(command.control().x, command.control().y,
                    command.point().x, command.point().y);
        break;
      case takram::graphics::CommandType::CONIC:
        path.conicTo(command.control().x, command.control().y,
                     command.point().x, command.point().y,
                     command.weight());
        break;
      case takram::graphics::CommandType::CUBIC:
        path.cubicTo(command.control1().x, command.control1().y,
                     command.control2().x, command.control2().y,
                     command.point().x, command.point().y);
        break;
      case takram::graphics::CommandType::CLOSE:
        path.close();
        break;
      default:
        assert(false);
        break;
    }
  }
  return std::move(path);
}

}  // namespace

takram::Shape2d GlyphStroker::operator()(const token::ufo::Glyph& glyph,
                                         const GlyphOutline& outline) const {
  GlyphStroker stroker(*this);
  takram::Shape2d shape;
  // Check for the number of contours of the resulting shape and retry if that
  // differs from the expected value, because the path simplification
  // occationally fails.
  bool success{};
  double shift{};
  bool negative{};
  for (; shift < shift_limit_; shift += shift_increment_) {
    for (negative = false; !negative; negative = !negative) {
      stroker.set_width(width_ + (negative ? -shift : shift));
      const auto bounds = outline.shape().bounds(true);
      shape = stroker.stroke(outline);
      shape = stroker.simplify(shape);
      if (!shape.bounds(true).contains(bounds)) {
        continue;
      }
      std::size_t contour_count{};
      std::size_t hole_count{};
      for (const auto& path : shape.paths()) {
        if (path.direction() != takram::PathDirection::UNDEFINED) {
          ++contour_count;
        }
        if (path.direction() == takram::PathDirection::COUNTER_CLOCKWISE) {
          ++hole_count;
        }
      }
      if (contour_count == glyph.lib->number_of_contours &&
          hole_count == glyph.lib->number_of_holes) {
        success = true;
        break;
      }
    }
    if (success) {
      break;
    }
  }
  if (success) {
    if (shift) {
      std::cerr << "Stroking succeeded by shifting stroke width by " <<
          (negative ? -shift : shift) <<
          " " << "(" << glyph.name << ")" << std::endl;
    }
    // CFF Opentype accepts only lines and cubic bezier paths, so we need to
    // convert conic curves to quadratic curves, which is approximation,
    // and then convert losslessly quadratic curves to cubic curves.
    shape.convertConicsToQuadratics();
    shape.convertQuadraticsToCubics();
    shape.removeDuplicates(1.0);
  } else {
    std::cerr << "Stroking failed by shifting stroke width up to ±" <<
        shift_limit_ << " " << "(" << glyph.name << ")" << std::endl;
    shape.reset();
  }
  return std::move(shape);
}

takram::Shape2d GlyphStroker::stroke(const GlyphOutline& outline) const {
  GlyphStroker stroker(*this);
  takram::Shape2d result;
  for (const auto& path : outline.shape().paths()) {
    const auto cap = outline.cap(path);
    if (cap != Cap::UNDEFINED) {
      stroker.set_cap(outline.cap(path));
    } else {
      stroker.set_cap(cap_);
    }
    const auto filled = outline.filled(path);
    stroker.set_filled(stroker.filled() || filled);
    const auto shape = stroker.stroke(path);
    for (const auto& path : shape.paths()) {
      result.paths().emplace_back(path);
    }
  }
  return std::move(result);
}

takram::Shape2d GlyphStroker::stroke(const takram::Shape2d& shape) const {
  takram::Shape2d result;
  for (const auto& path : shape.paths()) {
    const auto shape = stroke(path);
    for (const auto& path : shape.paths()) {
      result.paths().emplace_back(path);
    }
  }
  return std::move(result);
}

takram::Shape2d GlyphStroker::stroke(const takram::Path2d& path) const {
  SkPaint paint;
  if (filled_) {
    paint.setStyle(SkPaint::kStrokeAndFill_Style);
  } else {
    paint.setStyle(SkPaint::kStroke_Style);
  }
  paint.setStrokeWidth(width_);
  paint.setStrokeMiter(miter_);
  paint.setStrokeCap(convertCap(cap_));
  paint.setStrokeJoin(convertJoin(join_));
  SkPath sk_result;
  paint.getFillPath(convertPath(path), &sk_result, nullptr, precision_);
  auto result = convertShape(sk_result);
  if (filled_ && result.size() > 1) {
    // Take a path which has the largest bounding box when the path is filled.
    auto& paths = result.paths();
    auto max_path = std::begin(paths);
    auto max_bounds = max_path->bounds(true);
    for (auto itr = std::next(max_path); itr != std::end(paths); ++itr) {
      const auto bounds = itr->bounds(true);
      if (bounds.contains(max_bounds)) {
        max_path = itr;
        max_bounds = bounds;
      }
    }
    result = takram::Shape2d(*max_path);
  }
  return std::move(result);
}

takram::Shape2d GlyphStroker::simplify(const takram::Shape2d& shape) const {
  SkPath sk_path(convertShape(shape));
  SkPath sk_result;
  Simplify(sk_path, &sk_result);
  auto result = convertShape(sk_result);

  // Fix up winding rules
  const auto bounds_error = 1.0;
  const auto bounds_insets = width_ - bounds_error;
  std::unordered_map<takram::Path2d *, int> depths;
  auto& paths = result.paths();
  if (paths.size() == 1) {
    depths[&paths.front()] = 0;
  } else {
    for (auto itr = std::begin(paths); itr != std::end(paths);) {
      if (itr->direction() == takram::PathDirection::UNDEFINED) {
        itr = paths.erase(itr);
      } else {
        for (auto& other : result.paths()) {
          if (&*itr != &other) {
            // Because we assume the shape is stroked, every contour which
            // contains another must have the bounding box that is larger by
            // the stroke width with some amount of error.
            auto bounds = other.bounds(true);
            bounds.x += bounds_insets;
            bounds.y += bounds_insets;
            bounds.width -= bounds_insets + bounds_insets;
            bounds.height -= bounds_insets + bounds_insets;
            depths[&*itr] += bounds.contains(itr->bounds());
          }
        }
        ++itr;
      }
    }
  }
  for (const auto& pair : depths) {
    auto& path = *pair.first;
    if (pair.second % 2) {
      if (path.direction() == takram::PathDirection::CLOCKWISE) {
        path.reverse();
        assert(path.direction() == takram::PathDirection::COUNTER_CLOCKWISE);
      }
    } else if (path.direction() == takram::PathDirection::COUNTER_CLOCKWISE) {
      path.reverse();
      assert(path.direction() == takram::PathDirection::CLOCKWISE);
    }
  }
  return std::move(result);
}

}  // namespace token
