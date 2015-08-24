//
//  token/ufo/glif/guideline.h
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
#ifndef TOKEN_UFO_GLIF_GUIDELINE_H_
#define TOKEN_UFO_GLIF_GUIDELINE_H_

#include <string>

#include "token/ufo/number.h"

namespace token {
namespace ufo {
namespace glif {

class Guideline final {
 public:
  Guideline() = default;

  // Copy semantics
  Guideline(const Guideline& other) = default;
  Guideline& operator=(const Guideline& other) = default;

  // Comparison
  bool operator==(const Guideline& other) const;
  bool operator!=(const Guideline& other) const;

 public:
  Number x;
  Number y;
  Number angle;
  std::string name;
  std::string color;
  std::string identifier;
};

#pragma mark -

#pragma mark Comparison

inline bool Guideline::operator==(const Guideline& other) const {
  return (x == other.x &&
          y == other.y &&
          angle == other.angle &&
          name == other.name &&
          color == other.color &&
          identifier == other.identifier);
}

inline bool Guideline::operator!=(const Guideline& other) const {
  return !operator==(other);
}

}  // namespace glif

using glif::Guideline;

}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_GLIF_GUIDELINE_H_
