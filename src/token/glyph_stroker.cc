//
//  token/glyph_stroker.cc
//
//  The MIT License
//
//  Copyright (C) 2015 Shota Matsuda
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
#include <utility>
#include <vector>

#include "SkPaint.h"
#include "SkPath.h"
#include "SkPathOps.h"

#include "takram/math.h"
#include "takram/graphics.h"
#include "token/glyph_outline.h"

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

takram::Shape2d GlyphStroker::stroke(const GlyphOutline& outline) const {
  takram::Shape2d shape;
  const auto initial_cap = cap_;
  for (const auto& path : outline.shape().paths()) {
    const auto cap = outline.cap(path);
    if (cap != Cap::UNDEFINED) {
      cap_ = outline.cap(path);
    } else {
      cap_ = initial_cap;
    }
    const auto stroked_shape = stroke(path);
    for (const auto& path : stroked_shape.paths()) {
      shape.paths().emplace_back(path);
    }
  }
  return std::move(shape);
}

takram::Shape2d GlyphStroker::stroke(const takram::Path2d& path) const {
  SkPaint paint;
  paint.setStyle(SkPaint::kStroke_Style);
  paint.setStrokeWidth(width_);
  paint.setStrokeMiter(miter_);
  paint.setStrokeCap(convertCap(cap_));
  paint.setStrokeJoin(convertJoin(join_));
  SkPath result;
  SkPath stroke;
  stroke.rewind();
  paint.getFillPath(convertPath(path), &stroke, nullptr, precision_);
  result.addPath(stroke);
  return convertShape(result);
}

takram::Shape2d GlyphStroker::stroke(const takram::Shape2d& shape) const {
  SkPaint paint;
  paint.setStyle(SkPaint::kStroke_Style);
  paint.setStrokeWidth(width_);
  paint.setStrokeMiter(miter_);
  paint.setStrokeCap(convertCap(cap_));
  paint.setStrokeJoin(convertJoin(join_));
  SkPath result;
  SkPath stroke;
  for (const auto& path : shape.paths()) {
    stroke.rewind();
    paint.getFillPath(convertPath(path), &stroke, nullptr, precision_);
    result.addPath(stroke);
  }
  return convertShape(result);
}

takram::Shape2d GlyphStroker::simplify(const takram::Shape2d& shape) const {
  SkPath path(convertShape(shape));
  SkPath result;
  Simplify(path, &result);
  SkPath difference = result;
  difference.setFillType(SkPath::FillType::kWinding_FillType);
  Op(difference, result, SkPathOp::kDifference_SkPathOp, &difference);
  takram::Shape2d result_shape(convertShape(result));
  takram::Shape2d difference_shape(convertShape(difference));
  for (auto& result_path : result_shape.paths()) {
    for (auto& difference_path : difference_shape.paths()) {
      if (result_path.bounds() == difference_path.bounds()) {
        result_path.reverse();
      }
    }
  }
  return std::move(result_shape);
}

}  // namespace token
