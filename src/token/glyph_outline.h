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

#pragma once
#ifndef TOKEN_GLYPH_OUTLINE_H_
#define TOKEN_GLYPH_OUTLINE_H_

#include <cstddef>
#include <iterator>
#include <unordered_map>

#include "shotamatsuda/graphics.h"
#include "token/glyph_stroker.h"
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
  GlyphStroker::Cap cap(const shota::Path2d& path) const;
  GlyphStroker::Join join(const shota::Path2d& path) const;
  GlyphStroker::Align align(const shota::Path2d& path) const;
  bool filled(const shota::Path2d& path) const;

  // Conversion
  ufo::Glyph glyph(const ufo::Glyph& prototype) const;

 private:
  void processContour(const ufo::glif::Contour& contour);
  void processAttributes(const ufo::glif::Point& point);
  void processPath(const shota::Path2d& path, ufo::Glyph *glyph) const;

 public:
  shota::Shape2d shape_;
  std::unordered_map<std::size_t, GlyphStroker::Cap> caps_;
  std::unordered_map<std::size_t, GlyphStroker::Join> joins_;
  std::unordered_map<std::size_t, GlyphStroker::Align> aligns_;
  std::unordered_map<std::size_t, bool> filleds_;
};

// MARK: -

// MARK: Attributes

inline GlyphStroker::Cap GlyphOutline::cap(const shota::Path2d& path) const {
  const auto& paths = shape_.paths();
  const auto itr = std::find(std::begin(paths), std::end(paths), path);
  if (itr == std::end(paths)) {
    return GlyphStroker::Cap::UNDEFINED;
  }
  const auto index = std::distance(std::begin(paths), itr);
  return caps_.at(index);
}

inline GlyphStroker::Join GlyphOutline::join(const shota::Path2d& path) const {
  const auto& paths = shape_.paths();
  const auto itr = std::find(std::begin(paths), std::end(paths), path);
  if (itr == std::end(paths)) {
    return GlyphStroker::Join::UNDEFINED;
  }
  const auto index = std::distance(std::begin(paths), itr);
  return joins_.at(index);
}

inline GlyphStroker::Align GlyphOutline::align(
    const shota::Path2d& path) const {
  const auto& paths = shape_.paths();
  const auto itr = std::find(std::begin(paths), std::end(paths), path);
  if (itr == std::end(paths)) {
    return GlyphStroker::Align::UNDEFINED;
  }
  const auto index = std::distance(std::begin(paths), itr);
  return aligns_.at(index);
}

inline bool GlyphOutline::filled(const shota::Path2d& path) const {
  const auto& paths = shape_.paths();
  const auto itr = std::find(std::begin(paths), std::end(paths), path);
  if (itr == std::end(paths)) {
    return false;
  }
  const auto index = std::distance(std::begin(paths), itr);
  return filleds_.at(index);
}

}  // namespace token

#endif  // TOKEN_GLYPH_OUTLINE_H_
