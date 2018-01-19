// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#pragma once
#ifndef TOKEN_GLYPH_OUTLINE_H_
#define TOKEN_GLYPH_OUTLINE_H_

#include <cstddef>
#include <iterator>
#include <unordered_map>

#include "shotamatsuda/graphics.h"
#include "token/glyph_stroker.h"
#include "token/types.h"
#include "token/ufo/glif.h"
#include "token/ufo/glyph.h"

namespace token {

namespace shota = shotamatsuda;

class GlyphOutline final {
 public:
  GlyphOutline() = default;
  explicit GlyphOutline(const ufo::Glyph& glyph);

  // Copy semantics
  GlyphOutline(const GlyphOutline&) = default;
  GlyphOutline& operator=(const GlyphOutline&) = default;

  // Attributes
  const shota::Shape2d& shape() const { return shape_; }
  shota::Shape2d& shape() { return shape_; }
  Cap cap(const shota::Path2d& path) const;
  Join join(const shota::Path2d& path) const;
  Align align(const shota::Path2d& path) const;
  bool filled(const shota::Path2d& path) const;

  // Conversion
  ufo::Glyph glyph(const ufo::Glyph& prototype) const;

 private:
  void processContour(const ufo::glif::Contour& contour);
  void processStyle(const ufo::glif::ContourStyle& style);
  void processPath(const shota::Path2d& path, ufo::Glyph& glyph) const;

 public:
  shota::Shape2d shape_;
  std::unordered_map<std::size_t, Cap> caps_;
  std::unordered_map<std::size_t, Join> joins_;
  std::unordered_map<std::size_t, Align> aligns_;
  std::unordered_map<std::size_t, bool> filleds_;
};

// MARK: -

// MARK: Attributes

inline Cap GlyphOutline::cap(const shota::Path2d& path) const {
  const auto& paths = shape_.paths();
  const auto itr = std::find(std::begin(paths), std::end(paths), path);
  if (itr != std::end(paths)) {
    const auto index = std::distance(std::begin(paths), itr);
    if (caps_.find(index) != std::end(caps_)) {
      return caps_.at(index);
    }
  }
  return Cap::UNDEFINED;
}

inline Join GlyphOutline::join(const shota::Path2d& path) const {
  const auto& paths = shape_.paths();
  const auto itr = std::find(std::begin(paths), std::end(paths), path);
  if (itr != std::end(paths)) {
    const auto index = std::distance(std::begin(paths), itr);
    if (joins_.find(index) != std::end(joins_)) {
      return joins_.at(index);
    }
  }
  return Join::UNDEFINED;
}

inline Align GlyphOutline::align(
    const shota::Path2d& path) const {
  const auto& paths = shape_.paths();
  const auto itr = std::find(std::begin(paths), std::end(paths), path);
  if (itr != std::end(paths)) {
    const auto index = std::distance(std::begin(paths), itr);
    if (aligns_.find(index) != std::end(aligns_)) {
      return aligns_.at(index);
    }
  }
  return Align::UNDEFINED;
}

inline bool GlyphOutline::filled(const shota::Path2d& path) const {
  const auto& paths = shape_.paths();
  const auto itr = std::find(std::begin(paths), std::end(paths), path);
  if (itr != std::end(paths)) {
    const auto index = std::distance(std::begin(paths), itr);
    if (filleds_.find(index) != std::end(filleds_)) {
      return filleds_.at(index);
    }
  }
  return false;
}

}  // namespace token

#endif  // TOKEN_GLYPH_OUTLINE_H_
