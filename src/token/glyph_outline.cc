//
//  token/glyph_outline.cc
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

#include "token/glyph_outline.h"

#include <cassert>
#include <iterator>

#include "takram/graphics.h"
#include "token/ufo/glif/contour.h"
#include "token/ufo/glif/glyph.h"
#include "token/ufo/glif/point.h"

namespace token {

GlyphOutline::GlyphOutline(const ufo::Glyph& glyph) {
  if (!glyph.outline.first || glyph.outline.second.contours.empty()) {
    return;
  }
  for (auto& contour : glyph.outline.second.contours) {
    if (!contour.points.empty()) {
      processContour(contour);
    }
  }
}

void GlyphOutline::processContour(const ufo::Contour& contour) {
  const auto begin = std::begin(contour.points);
  const auto end = std::end(contour.points);
  assert(begin != end);
  auto itr = begin;
  const auto& point = *itr;
  // When a contour starts with a move point, it signifies an open contour.
  const bool open = (point.type == ufo::Point::Type::MOVE);
  shape_.moveTo(point.x, point.y);
  ++itr;
  auto offcurve1 = end;
  auto offcurve2 = end;
  for (;; ++itr) {
    if (itr == end) {
      if (open) {
        break;  // This is the end of an open contour
      }
      itr = begin;
    }
    const auto& point = *itr;
    switch (point.type) {
      case ufo::Point::Type::MOVE:
        assert(false);  // A point of move must apear the first.
        break;
      case ufo::Point::Type::LINE:
        shape_.lineTo(point.x, point.y);
        offcurve1 = offcurve2 = end;
        break;
      case ufo::Point::Type::OFFCURVE:
        if (offcurve1 == end) {
          offcurve1 = itr;
        } else if (offcurve2 == end) {
          offcurve2 = itr;
        } else {
          assert(false);
        }
        break;
      case ufo::Point::Type::CURVE:
        if (offcurve1 == end) {
          shape_.lineTo(point.x, point.y);
        } else if (offcurve2 == end) {
          shape_.quadraticTo(offcurve2->x, offcurve2->y, point.x, point.y);
        } else {
          shape_.cubicTo(offcurve1->x, offcurve1->y,
                         offcurve2->x, offcurve2->y,
                         point.x, point.y);
        }
        offcurve1 = offcurve2 = end;
        break;
      case ufo::glif::Point::Type::QCURVE:
        assert(false);  // Not supported
        break;
      default:
        assert(false);
        break;
    }
    if (itr == begin) {
      break;  // This is the end of a closed contour
    }
  }
}

}  // namespace token
