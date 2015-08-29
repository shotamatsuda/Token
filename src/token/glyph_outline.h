//
//  token/glyph_outline.h
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

#pragma once
#ifndef TOKEN_GLYPH_OUTLINE_H_
#define TOKEN_GLYPH_OUTLINE_H_

#include <cstddef>
#include <iterator>
#include <unordered_map>

#include "takram/graphics.h"
#include "takram/math.h"
#include "token/glyph_stroker.h"
#include "token/ufo/glif/contour.h"
#include "token/ufo/glif/glyph.h"

namespace token {

class GlyphOutline final {
 public:
  GlyphOutline() = default;
  explicit GlyphOutline(const ufo::Glyph& glyph);
  GlyphOutline(const ufo::Glyph& glyph, const takram::Shape2d& shape);

  // Copy semantics
  GlyphOutline(const GlyphOutline& other) = default;
  GlyphOutline& operator=(const GlyphOutline& other) = default;

  // Attributes
  const ufo::Glyph& glyph() const { return glyph_; }
  const takram::Shape2d& shape() const { return shape_; }
  GlyphStroker::Cap cap(const takram::Path2d& path) const;

 private:
  void processContour(const ufo::Contour& contour);
  void processPath(const takram::Path2d& path);

 public:
  ufo::Glyph glyph_;
  takram::Shape2d shape_;
  std::unordered_map<std::size_t, GlyphStroker::Cap> caps_;
};

#pragma mark -

#pragma mark Attributes

inline GlyphStroker::Cap GlyphOutline::cap(const takram::Path2d& path) const {
  const auto& paths = shape_.paths();
  const auto itr = std::find(std::begin(paths), std::end(paths), path);
  if (itr == std::end(paths)) {
    return GlyphStroker::Cap::UNDEFINED;
  }
  const auto index = std::distance(std::begin(paths), itr);
  return caps_.at(index);
}

}  // namespace token

#endif  // TOKEN_GLYPH_OUTLINE_H_
