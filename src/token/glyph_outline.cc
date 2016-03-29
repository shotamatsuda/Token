//
//  token/glyph_outline.cc
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

#include "token/glyph_outline.h"

#include <algorithm>
#include <cassert>
#include <deque>
#include <iterator>
#include <string>
#include <unordered_set>

#include <boost/algorithm/string.hpp>

#include "takram/graphics.h"
#include "token/glyph_stroker.h"
#include "token/ufo/glif.h"
#include "token/ufo/glyph.h"

namespace token {

GlyphOutline::GlyphOutline(const ufo::Glyph& glyph) {
  if (!glyph.outline.exists() || glyph.outline->contours.empty()) {
    return;
  }
  for (const auto& contour : glyph.outline->contours) {
    if (!contour.points.empty()) {
      processContour(contour);
    }
  }
}

#pragma mark Conversion

ufo::Glyph GlyphOutline::glyph(const ufo::Glyph& prototype) const {
  ufo::Glyph result(prototype);
  if (result.outline.exists()) {
    result.outline->contours.clear();
  }
  result.outline.emplace();
  for (const auto& path : shape_.paths()) {
    if (!shape_.empty()) {
      processPath(path, &result);
    }
  }
  return std::move(result);
}

void GlyphOutline::processContour(const ufo::glif::Contour& contour) {
  const auto begin = std::begin(contour.points);
  const auto end = std::end(contour.points);
  assert(begin != end);
  auto itr = begin;

  // When a contour starts with a move point, it signifies an open contour.
  const bool open = (itr->type == ufo::glif::Point::Type::MOVE);
  auto close = begin;
  auto offcurve1 = end;
  auto offcurve2 = end;
  if (itr->type == ufo::glif::Point::Type::MOVE) {
    shape_.moveTo(itr->x, itr->y);
    ++itr;
  } else {
    while (itr->type == ufo::glif::Point::Type::OFFCURVE && itr != end) {
      ++itr;
    }
    if (itr != end) {
      shape_.moveTo(itr->x, itr->y);
      close = itr++;
    }
  }
  for (;; ++itr) {
    if (itr == end) {
      if (open) {
        break;  // This is the end of an open contour
      }
      itr = begin;
    }
    switch (itr->type) {
      case ufo::glif::Point::Type::MOVE:
        assert(false);  // A move point must appear the first
        break;
      case ufo::glif::Point::Type::LINE:
        shape_.lineTo(itr->x, itr->y);
        offcurve1 = offcurve2 = end;
        break;
      case ufo::glif::Point::Type::OFFCURVE:
        if (offcurve1 == end) {
          offcurve1 = itr;
        } else if (offcurve2 == end) {
          offcurve2 = itr;
        } else {
          assert(false);
        }
        break;
      case ufo::glif::Point::Type::CURVE:
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
    if (itr == close) {
      break;  // This is the end of a closed contour
    }
  }
  processAttributes(contour.points.back());
}

void GlyphOutline::processAttributes(const ufo::glif::Point& point) {
  std::unordered_set<std::string> attributes;
  boost::algorithm::split(attributes, point.name, boost::is_any_of(" ,"));
  auto cap = GlyphStroker::Cap::UNDEFINED;
  if (attributes.count("cap-butt")) {
    cap = GlyphStroker::Cap::BUTT;
  } else if (attributes.count("cap-round")) {
    cap = GlyphStroker::Cap::ROUND;
  } else if (attributes.count("cap-project")) {
    cap = GlyphStroker::Cap::PROJECT;
  }
  caps_.emplace(shape_.paths().size() - 1, cap);
  auto join = GlyphStroker::Join::UNDEFINED;
  if (attributes.count("join-miter")) {
    join = GlyphStroker::Join::MITER;
  } else if (attributes.count("join-round")) {
    join = GlyphStroker::Join::ROUND;
  } else if (attributes.count("join-bevel")) {
    join = GlyphStroker::Join::BEVEL;
  }
  joins_.emplace(shape_.paths().size() - 1, join);
  auto align = GlyphStroker::Align::UNDEFINED;
  if (attributes.count("align-left")) {
    align = GlyphStroker::Align::LEFT;
  } else if (attributes.count("align-right")) {
    align = GlyphStroker::Align::RIGHT;
  }
  aligns_.emplace(shape_.paths().size() - 1, align);
  bool filled{};
  if (attributes.count("filled")) {
    filled = true;
  }
  filleds_.emplace(shape_.paths().size() - 1, filled);
}

void GlyphOutline::processPath(const takram::Path2d& path,
                               ufo::Glyph *glyph) const {
  assert(!path.empty());
  assert(glyph);
  std::deque<ufo::glif::Point> points;
  for (const auto& command : path) {
    switch (command.type()) {
      case takram::graphics::CommandType::MOVE:
        points.emplace_back(command.point().x,
                            command.point().y,
                            ufo::glif::Point::Type::MOVE);
        break;
      case takram::graphics::CommandType::LINE:
        points.emplace_back(command.point().x,
                            command.point().y,
                            ufo::glif::Point::Type::LINE);
        break;
      case takram::graphics::CommandType::QUADRATIC:
        assert(false);  // Not supported
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
                            ufo::glif::Point::Type::CURVE);
        break;
      case takram::graphics::CommandType::CLOSE:
        break;  // We'll check this later
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
    points.push_front(back);
  }
  if (!points.empty()) {
    glyph->outline->contours.emplace_back();
    auto& target = glyph->outline->contours.back().points;
    target.resize(points.size());
    std::copy(std::begin(points), std::end(points), std::begin(target));
  }
}

}  // namespace token
