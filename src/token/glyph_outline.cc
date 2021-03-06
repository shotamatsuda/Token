// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#include "token/glyph_outline.h"

#include <algorithm>
#include <cassert>
#include <deque>
#include <exception>
#include <iterator>
#include <string>
#include <unordered_set>

#include <boost/algorithm/string.hpp>

#include "shotamatsuda/graphics.h"
#include "token/glyph_stroker.h"
#include "token/types.h"
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
      if (!glyph.lib.exists()) {
        continue;
      }
      const auto style = glyph.lib->contour_styles.find(contour.identifier);
      if (!style) {
        continue;
      }
      processStyle(*style);
    }
  }
}

// MARK: Conversion

ufo::Glyph GlyphOutline::glyph(const ufo::Glyph& prototype) const {
  ufo::Glyph result(prototype);
  if (result.outline.exists()) {
    result.outline->contours.clear();
  }
  result.outline.emplace();
  for (const auto& path : shape_.paths()) {
    if (!shape_.empty()) {
      processPath(path, result);
    }
  }
  return result;
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
        throw std::runtime_error("Move point must appear the first");
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
          throw std::runtime_error("Extraneous offcurve point");
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
        throw std::runtime_error("Quadratic curve point is not supported");
      default:
        throw std::runtime_error("Invalid type of point");
    }
    if (itr == close) {
      break;  // This is the end of a closed contour
    }
  }
}

void GlyphOutline::processStyle(const ufo::glif::ContourStyle& style) {
  const auto index = shape_.paths().size() - 1;
  caps_.emplace(index, style.cap);
  joins_.emplace(index, style.join);
  aligns_.emplace(index, style.align);
  filleds_.emplace(index, style.filled);
}

void GlyphOutline::processPath(const shota::Path2d& path,
                               ufo::Glyph& glyph) const {
  assert(!path.empty());
  std::deque<ufo::glif::Point> points;
  for (const auto& command : path) {
    switch (command.type()) {
      case shota::graphics::CommandType::MOVE:
        points.emplace_back(command.point().x,
                            command.point().y,
                            ufo::glif::Point::Type::MOVE);
        break;
      case shota::graphics::CommandType::LINE:
        points.emplace_back(command.point().x,
                            command.point().y,
                            ufo::glif::Point::Type::LINE);
        break;
      case shota::graphics::CommandType::QUADRATIC:
        assert(false);  // Not supported
        break;
      case shota::graphics::CommandType::CONIC:
        assert(false);  // Not supported
        break;
      case shota::graphics::CommandType::CUBIC:
        points.emplace_back(command.control1().x,
                            command.control1().y);
        points.emplace_back(command.control2().x,
                            command.control2().y);
        points.emplace_back(command.point().x,
                            command.point().y,
                            ufo::glif::Point::Type::CURVE);
        break;
      case shota::graphics::CommandType::CLOSE:
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
    glyph.outline->contours.emplace_back();
    auto& target = glyph.outline->contours.back().points;
    target.resize(points.size());
    std::copy(std::begin(points), std::end(points), std::begin(target));
  }
}

}  // namespace token
