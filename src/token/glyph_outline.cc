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

#include <algorithm>
#include <cassert>
#include <deque>
#include <iterator>

#include "takram/graphics.h"
#include "token/ufo/glif/contour.h"
#include "token/ufo/glif/glyph.h"
#include "token/ufo/glif/point.h"

namespace token {

GlyphOutline::GlyphOutline(const ufo::Glyph& glyph) : glyph_(glyph) {
  if (!glyph_.outline.first || glyph_.outline.second.contours.empty()) {
    return;
  }
  for (const auto& contour : glyph_.outline.second.contours) {
    if (!contour.points.empty()) {
      processContour(contour);
    }
  }
}

GlyphOutline::GlyphOutline(const ufo::Glyph& glyph,
                           const takram::Shape2d& shape)
    : glyph_(glyph),
      shape_(shape) {
  if (glyph_.outline.first) {
    glyph_.outline.second.contours.clear();
  }
  for (const auto& path : shape.paths()) {
    if (!shape.empty()) {
      glyph_.outline.first = true;
      processPath(path);
    }
  }
}

void GlyphOutline::processContour(const ufo::Contour& contour) {
  const auto begin = std::begin(contour.points);
  const auto end = std::end(contour.points);
  assert(begin != end);
  auto itr = begin;

  // When a contour starts with a move point, it signifies an open contour.
  const bool open = (itr->type == ufo::Point::Type::MOVE);
  shape_.moveTo(itr->x, itr->y);
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
    switch (itr->type) {
      case ufo::Point::Type::MOVE:
        assert(false);  // A point of move must apear the first
        break;
      case ufo::Point::Type::LINE:
        shape_.lineTo(itr->x, itr->y);
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
          shape_.lineTo(itr->x, itr->y);
        } else if (offcurve2 == end) {
          shape_.quadraticTo(offcurve2->x, offcurve2->y, itr->x, itr->y);
        } else {
          shape_.cubicTo(offcurve1->x, offcurve1->y,
                         offcurve2->x, offcurve2->y,
                         itr->x, itr->y);
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

void GlyphOutline::processPath(const takram::Path2d& path) {
  assert(!path.empty());
  std::deque<ufo::Point> points;
  for (const auto& command : path) {
    switch (command.type()) {
      case takram::graphics::CommandType::MOVE:
        points.emplace_back(command.point().x,
                            command.point().y,
                            ufo::Point::Type::MOVE);
        break;
      case takram::graphics::CommandType::LINE:
        points.emplace_back(command.point().x,
                            command.point().y,
                            ufo::Point::Type::LINE);
        break;
      case takram::graphics::CommandType::QUADRATIC:
        points.emplace_back(command.control().x,
                            command.control().y);
        points.emplace_back(command.point().x,
                            command.point().y,
                            ufo::Point::Type::CURVE);
        break;
      case takram::graphics::CommandType::CONIC:
        assert(false);  // Not supported
        break;
      case takram::graphics::CommandType::CUBIC:
        points.emplace_back(command.control1().x,
                            command.control1().y);
        points.emplace_back(command.control2().x,
                            command.control2().y);
        points.emplace_back(command.point().x,
                            command.point().y,
                            ufo::Point::Type::CURVE);
        break;
      case takram::graphics::CommandType::CLOSE:
        break;  // Ignore
      default:
        assert(false);
        break;
    }
  }
  if (path.closed()) {
    // Remove the first point of move type and move the point of the last
    // command to the first of points.
    points.pop_front();
    auto back = points.back();
    points.pop_back();
    points.push_front(std::move(back));
  }
  if (!points.empty()) {
    glyph_.outline.second.contours.emplace_back();
    auto& target = glyph_.outline.second.contours.back().points;
    target.resize(points.size());
    std::copy(std::begin(points), std::end(points), std::begin(target));
  }
}

}  // namespace token
