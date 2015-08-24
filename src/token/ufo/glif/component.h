//
//  token/ufo/glif/component.h
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
#ifndef TOKEN_UFO_GLIF_COMPONENT_H_
#define TOKEN_UFO_GLIF_COMPONENT_H_

#include <string>

#include "token/ufo/number.h"

namespace token {
namespace ufo {
namespace glif {

class Component final {
 public:
  Component();

  // Copy semantics
  Component(const Component& other) = default;
  Component& operator=(const Component& other) = default;

  // Comparison
  bool operator==(const Component& other) const;
  bool operator!=(const Component& other) const;

 public:
  std::string base;
  Number x_scale;
  Number xy_scale;
  Number yx_scale;
  Number y_scale;
  Number x_offset;
  Number y_offset;
  std::string identifier;
};

#pragma mark -

inline Component::Component()
    : x_scale(1),
      xy_scale(),
      yx_scale(),
      y_scale(1),
      x_offset(),
      y_offset() {}

#pragma mark Comparison

inline bool Component::operator==(const Component& other) const {
  return (base == other.base &&
          x_scale == other.x_scale &&
          xy_scale == other.xy_scale &&
          yx_scale == other.yx_scale &&
          y_scale == other.y_scale &&
          x_offset == other.x_offset &&
          y_offset == other.y_offset &&
          identifier == other.identifier);
}

inline bool Component::operator!=(const Component& other) const {
  return !operator==(other);
}

}  // namespace glif

using glif::Component;

}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_GLIF_COMPONENT_H_
