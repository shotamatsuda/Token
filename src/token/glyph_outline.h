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
#ifndef TOKEN_GLYPH_H_
#define TOKEN_GLYPH_H_

#include "takram/graphics.h"
#include "token/ufo/glif/contour.h"
#include "token/ufo/glif/glyph.h"

namespace token {

class GlyphOutline final {
 public:
  GlyphOutline() = default;
  explicit GlyphOutline(const ufo::Glyph& glyph);

  // Disallow copy semantics
  GlyphOutline(const GlyphOutline& other) = delete;
  GlyphOutline& operator=(const GlyphOutline& other) = delete;

  // Move semantics
  GlyphOutline(GlyphOutline&& other) = default;
  GlyphOutline& operator=(GlyphOutline&& other) = default;

  // Attributes
  const takram::Shape2d& shape() const { return shape_; }
  takram::Shape2d& shape() { return shape_; }

 private:
  void processContour(const ufo::Contour& contour);

 public:
  takram::Shape2d shape_;
};

#pragma mark -

}  // namespace token

#endif  // TOKEN_UFO_GLIF_GLYPH_H_
