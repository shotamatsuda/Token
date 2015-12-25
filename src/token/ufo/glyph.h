//
//  token/ufo/glyph.h
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
#ifndef TOKEN_UFO_GLYPH_H_
#define TOKEN_UFO_GLYPH_H_

#include <istream>
#include <ostream>
#include <string>
#include <vector>

#include <boost/property_tree/ptree.hpp>

#include "token/ufo/glif.h"

namespace token {
namespace ufo {

class Glyph final {
 public:
  Glyph() = default;
  explicit Glyph(const std::string& path);

  // Copy semantics
  Glyph(const Glyph&) = default;
  Glyph& operator=(const Glyph&) = default;

  // Opening and saving
  bool open(const std::string& path);
  bool open(std::istream *stream);
  bool save(const std::string& path) const;
  bool save(std::ostream *stream) const;

 public:
  std::string name;
  Optional<glif::Advance> advance;
  std::vector<glif::Unicode> unicodes;
  Optional<glif::Image> image;
  std::vector<glif::Guideline> guidelines;
  std::vector<glif::Anchor> anchors;
  Optional<glif::Outline> outline;
  Optional<glif::Lib> lib;
};

// Comparison
bool operator==(const Glyph& lhs, const Glyph& rhs);
bool operator!=(const Glyph& lhs, const Glyph& rhs);

#pragma mark -

inline Glyph::Glyph(const std::string& path) {
  open(path);
}

#pragma mark Comparison

inline bool operator==(const Glyph& lhs, const Glyph& rhs) {
  return (lhs.name == rhs.name &&
          lhs.advance == rhs.advance &&
          lhs.unicodes == rhs.unicodes &&
          lhs.image == rhs.image &&
          lhs.guidelines == rhs.guidelines &&
          lhs.anchors == rhs.anchors &&
          lhs.outline == rhs.outline &&
          lhs.lib == rhs.lib);
}

inline bool operator!=(const Glyph& lhs, const Glyph& rhs) {
  return !(lhs == rhs);
}

}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_GLYPH_H_
