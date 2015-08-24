//
//  token/skia.cc
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

#include "token/skia.h"

#include <cassert>
#include <utility>
#include <vector>

#include "SkPaint.h"
#include "SkPath.h"
#include "SkPathOps.h"

#include "takram/math.h"
#include "takram/graphics.h"

namespace token {

takram::Shape2d convertShape(const SkPath& other) {
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

SkPath convertShape(const takram::Shape2d& other) {
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

takram::Path2d convertPath(const SkPath& other) {
  takram::Path2d path;
  SkPath::RawIter itr(other);
  SkPath::Verb verb;
  std::vector<SkPoint> points(4);
  while ((verb = itr.next(points.data())) != SkPath::kDone_Verb) {
    switch (verb) {
      case SkPath::Verb::kMove_Verb:
        path.moveTo(points[0].x(), points[0].y());
        break;
      case SkPath::Verb::kLine_Verb:
        path.lineTo(points[1].x(), points[1].y());
        break;
      case SkPath::Verb::kQuad_Verb:
        path.quadraticTo(points[1].x(), points[1].y(),
                         points[2].x(), points[2].y());
        break;
      case SkPath::Verb::kConic_Verb:
        path.conicTo(points[1].x(), points[1].y(),
                     points[2].x(), points[2].y(),
                     itr.conicWeight());
        break;
      case SkPath::Verb::kCubic_Verb:
        path.cubicTo(points[1].x(), points[1].y(),
                     points[2].x(), points[2].y(),
                     points[3].x(), points[3].y());
        break;
      case SkPath::Verb::kClose_Verb:
        path.close();
        break;
      default:
        assert(false);
        break;
    }
  }
  return std::move(path);
}

SkPath convertPath(const takram::Path2d& other) {
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

}  // namespace token
