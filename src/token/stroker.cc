//
//  token/stroker.cc
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

#include "token/stroker.h"

#include <cassert>
#include <utility>

#include "SkPaint.h"
#include "SkPath.h"
#include "SkPathOps.h"

#include "takram/math.h"
#include "takram/graphics.h"

#include "token/skia.h"

namespace token {

namespace {

inline SkPaint::Cap convertCap(Stroker::Cap cap) {
  switch (cap) {
    case Stroker::Cap::BUTT:
      return SkPaint::Cap::kButt_Cap;
    case Stroker::Cap::ROUND:
      return SkPaint::Cap::kRound_Cap;
    case Stroker::Cap::PROJECT:
      return SkPaint::Cap::kSquare_Cap;
    default:
      assert(false);
      break;
  }
  return SkPaint::Cap::kDefault_Cap;
}

inline SkPaint::Join convertJoin(Stroker::Join join) {
  switch (join) {
    case Stroker::Join::MITER:
      return SkPaint::Join::kMiter_Join;
    case Stroker::Join::ROUND:
      return SkPaint::Join::kRound_Join;
    case Stroker::Join::BEVEL:
      return SkPaint::Join::kBevel_Join;
    default:
      assert(false);
      break;
  }
  return SkPaint::Join::kDefault_Join;
}

static void fixDirection(SkPath *stroke) {
  assert(stroke);
  takram::Shape2d shape(convertShape(*stroke));
  if (shape.size() == 1) {
    return;  // Nothing to fix
  }
  stroke->rewind();
  takram::Rect2d max_bounds;
  auto& paths = shape.paths();
  auto max_itr = paths.begin();
  for (auto itr = paths.begin(); itr != paths.end(); ++itr) {
    const auto& bounds = itr->bounds();
    if (bounds.area() > max_bounds.area()) {
      max_bounds = bounds;
      max_itr = itr;
    }
  }
  for (auto itr = paths.begin(); itr != paths.end(); ++itr) {
    if (itr != max_itr &&
        itr->direction() != takram::PathDirection::COUNTER_CLOCKWISE) {
      itr->reverse();
    }
  }
  *stroke = convertShape(shape);
}

}  // namespace

takram::Shape2d Stroker::operator()(const takram::Shape2d& shape) const {
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
    paint.getFillPath(convertPath(path), &stroke, nullptr, tolerance_);
//    Simplify(stroke, &stroke);
//    fixDirection(&stroke);
    result.addPath(stroke);
  }
  return convertShape(result);
}

}  // namespace token
