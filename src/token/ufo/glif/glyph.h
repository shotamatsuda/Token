//
//  token/ufo/glif/glyph.h
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
#ifndef TOKEN_UFO_GLIF_GLYPH_H_
#define TOKEN_UFO_GLIF_GLYPH_H_

#include <string>
#include <vector>

#include "token/ufo/glif/advance.h"
#include "token/ufo/glif/anchor.h"
#include "token/ufo/glif/guideline.h"
#include "token/ufo/glif/image.h"
#include "token/ufo/glif/outline.h"
#include "token/ufo/glif/unicode.h"

namespace token {
namespace ufo {
namespace glif {

class Glyph final {
 public:
  explicit Glyph(const std::string& name);

  // Copy semantics
  Glyph(const Glyph& other) = default;
  Glyph& operator=(const Glyph& other) = default;

  // Comparison
  bool operator==(const Glyph& other) const;
  bool operator!=(const Glyph& other) const;

 public:
  std::string name;
  Advance advance;
  std::vector<Unicode> unicodes;
  Image image;
  std::vector<Guideline> guidelines;
  std::vector<Anchor> anchors;
  Outline outline;
};

#pragma mark -

inline Glyph::Glyph(const std::string& name) : name(name) {}

#pragma mark Comparison

inline bool Glyph::operator==(const Glyph& other) const {
  return (name == other.name &&
          advance == other.advance &&
          unicodes == other.unicodes &&
          image == other.image &&
          guidelines == other.guidelines &&
          anchors == other.anchors &&
          outline == other.outline);
}

inline bool Glyph::operator!=(const Glyph& other) const {
  return operator==(other);
}

}  // namespace glif

using glif::Glyph;

}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_GLIF_GLYPH_H_
