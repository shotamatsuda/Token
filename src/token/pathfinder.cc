//
//  token/pathfinder.cc
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

#include "token/pathfinder.h"

#include <utility>

#include "SkPath.h"
#include "SkPathOps.h"

#include "takram/graphics.h"

#include "token/skia.h"

namespace token {

takram::Shape2d simplify(const takram::Shape2d& shape) {
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
        difference_path.reverse();
      }
    }
  }
  return std::move(result_shape);
}

}  // namespace token
