//
//  The MIT License
//
//  Copyright (C) 2015-2017 Shota Matsuda
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

#include "shotamatsuda/graphics.h"
#include "shotamatsuda/math.h"
#include "token/glyph_outline.h"
#include "token/ufo/font_info.h"
#include "token/ufo/glif/advance.h"
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

inline shota::Shape2d convertShape(const SkPath& other) {
  shota::Shape2d shape;
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
  return shape;
}

inline SkPath convertShape(const shota::Shape2d& other) {
  SkPath path;
  for (const auto& command : other) {
    switch (command.type()) {
      case shota::graphics::CommandType::MOVE:
        path.moveTo(command.point().x, command.point().y);
        break;
      case shota::graphics::CommandType::LINE:
        path.lineTo(command.point().x, command.point().y);
        break;
      case shota::graphics::CommandType::QUADRATIC:
        path.quadTo(command.control().x, command.control().y,
                    command.point().x, command.point().y);
        break;
      case shota::graphics::CommandType::CONIC:
        path.conicTo(command.control().x, command.control().y,
                     command.point().x, command.point().y,
                     command.weight());
        break;
      case shota::graphics::CommandType::CUBIC:
        path.cubicTo(command.control1().x, command.control1().y,
                     command.control2().x, command.control2().y,
                     command.point().x, command.point().y);
        break;
      case shota::graphics::CommandType::CLOSE:
        path.close();
        break;
      default:
        assert(false);
        break;
    }
  }
  return path;
}

inline SkPath convertPath(const shota::Path2d& other) {
  SkPath path;
  for (const auto& command : other) {
    switch (command.type()) {
      case shota::graphics::CommandType::MOVE:
        path.moveTo(command.point().x, command.point().y);
        break;
      case shota::graphics::CommandType::LINE:
        path.lineTo(command.point().x, command.point().y);
        break;
      case shota::graphics::CommandType::QUADRATIC:
        path.quadTo(command.control().x, command.control().y,
                    command.point().x, command.point().y);
        break;
      case shota::graphics::CommandType::CONIC:
        path.conicTo(command.control().x, command.control().y,
                     command.point().x, command.point().y,
                     command.weight());
        break;
      case shota::graphics::CommandType::CUBIC:
        path.cubicTo(command.control1().x, command.control1().y,
                     command.control2().x, command.control2().y,
                     command.point().x, command.point().y);
        break;
      case shota::graphics::CommandType::CLOSE:
        path.close();
        break;
      default:
        assert(false);
        break;
    }
  }
  return path;
}

}  // namespace

std::pair<shota::Shape2d, ufo::glif::Advance> GlyphStroker::operator()(
    const ufo::FontInfo& font_info,
    const ufo::Glyph& glyph,
    const GlyphOutline& outline) const {
  const auto scale = (font_info.cap_height - width_) / font_info.cap_height;
  const auto stroke_bounds = outline.shape().bounds(true);
  const auto lsb = stroke_bounds.minX();
  const auto rsb = glyph.advance->width - stroke_bounds.maxX();
  auto scaled_outline = outline;
  for (auto& command : scaled_outline.shape()) {
    command.point() *= scale;
    command.control1() *= scale;
    command.control2() *= scale;
  }

  // CFF Opentype accepts only lines and cubic bezier paths, so we need to
  // convert conic curves to quadratic curves, which is approximation,
  // and then convert losslessly quadratic curves to cubic curves.
  auto shape = stroke(glyph, scaled_outline);
  shape.convertConicsToQuadratics();
  shape.convertQuadraticsToCubics();
  shape.removeDuplicates(1.0);

  // Offset the resulting shape maintains LSB and RSB of the original shape.
  // Because the advance must be integral, its rounding error should be added
  // to both LSB and RSB proportionally.
  const auto bounds = shape.bounds(true);
  const auto advance = lsb + bounds.width + rsb;
  const auto rounded_advance = std::round(advance);
  const auto error = rounded_advance - advance;
  double lsb_error{};
  if (lsb + rsb) {
    lsb_error = error * lsb / (lsb + rsb);
  } else {
    lsb_error = error / 2.0;
  }
  shota::Vec2d offset(lsb + lsb_error - bounds.minX(), width_ / 2.0);
  for (auto& command : shape) {
    command.point() += offset;
    command.control1() += offset;
    command.control2() += offset;
  }
  auto glyph_advance = *glyph.advance;
  glyph_advance.width = rounded_advance;
  return std::make_pair(shape, glyph_advance);
}

shota::Shape2d GlyphStroker::stroke(const ufo::Glyph& glyph,
                                     const GlyphOutline& outline) const {
  GlyphStroker stroker(*this);
  shota::Shape2d shape;
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
        if (path.direction() != shota::PathDirection::UNDEFINED) {
          ++contour_count;
        }
        if (path.direction() == shota::PathDirection::COUNTER_CLOCKWISE) {
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
#ifdef DEBUG
    if (shift) {
      std::cerr << "Stroking succeeded by shifting stroke width by " <<
          (negative ? -shift : shift) <<
          " " << "(" << glyph.name << ")" << std::endl;
    }
#endif
  } else {
#ifdef DEBUG
    std::cerr << "Stroking failed by shifting stroke width up to ±" <<
        shift_limit_ << " " << "(" << glyph.name << ")" << std::endl;
#endif
    shape.reset();
  }
  return shape;
}

shota::Shape2d GlyphStroker::stroke(const GlyphOutline& outline) const {
  GlyphStroker stroker(*this);
  shota::Shape2d result;
  for (const auto& path : outline.shape().paths()) {
    if (outline.cap(path) != Cap::UNDEFINED) {
      stroker.set_cap(outline.cap(path));
    } else {
      stroker.set_cap(cap_);
    }
    if (outline.join(path) != Join::UNDEFINED) {
      stroker.set_join(outline.join(path));
    } else {
      stroker.set_join(join_);
    }
    if (outline.align(path) != Align::UNDEFINED) {
      stroker.set_align(outline.align(path));
    } else {
      stroker.set_align(align_);
    }
    stroker.set_filled(stroker.filled() || outline.filled(path));
    const auto shape = stroker.stroke(path);
    for (const auto& path : shape.paths()) {
      result.paths().emplace_back(path);
    }
  }
  return result;
}

shota::Shape2d GlyphStroker::stroke(const shota::Path2d& path) const {
  auto aligned_path = path;
  shota::Vec2d offset;
  switch (align_) {
    case Align::LEFT:
      offset.x = width_ / 2.0;
      break;
    case Align::RIGHT:
      offset.x = -width_ / 2.0;
      break;
    default:
      break;
  }
  if (!offset.empty()) {
    for (auto& command : aligned_path) {
      command.point() += offset;
      command.control1() += offset;
      command.control2() += offset;
    }
  }
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
  paint.getFillPath(convertPath(aligned_path), &sk_result, nullptr, precision_);
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
    result = shota::Shape2d(*max_path);
  }
  return result;
}

shota::Shape2d GlyphStroker::simplify(const shota::Shape2d& shape) const {
  SkPath sk_path(convertShape(shape));
  SkPath sk_result;
  Simplify(sk_path, &sk_result);
  auto result = convertShape(sk_result);

  // Fix up winding rules
  const auto bounds_error = 1.0;
  const auto bounds_insets = width_ - bounds_error;
  std::unordered_map<shota::Path2d *, int> depths;
  auto& paths = result.paths();
  if (paths.size() == 1) {
    depths[&paths.front()] = 0;
  } else {
    for (auto itr = std::begin(paths); itr != std::end(paths);) {
      if (itr->direction() == shota::PathDirection::UNDEFINED) {
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
      if (path.direction() == shota::PathDirection::CLOCKWISE) {
        path.reverse();
        assert(path.direction() == shota::PathDirection::COUNTER_CLOCKWISE);
      }
    } else if (path.direction() == shota::PathDirection::COUNTER_CLOCKWISE) {
      path.reverse();
      assert(path.direction() == shota::PathDirection::CLOCKWISE);
    }
  }
  return result;
}

}  // namespace token
