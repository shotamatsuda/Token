//
//  token/ufo/guideline.h
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

#pragma once
#ifndef TOKEN_UFO_GUIDELINE_H_
#define TOKEN_UFO_GUIDELINE_H_

#include <string>

#include "token/ufo/property_list.h"

namespace token {
namespace ufo {

class Guideline final {
 public:
  Guideline() = default;
  Guideline(double x,
            double y,
            double angle,
            const std::string& name,
            const std::string& color,
            const std::string& identifier);

  // Copy semantics
  Guideline(const Guideline&) = default;
  Guideline& operator=(const Guideline&) = default;

  // Property list
  static Guideline read(const PropertyList& plist);
  PropertyList plist() const;

 public:
  double x;
  double y;
  double angle;
  std::string name;
  std::string color;
  std::string identifier;
};

// Comparison
bool operator==(const Guideline& lhs, const Guideline& rhs);
bool operator!=(const Guideline& lhs, const Guideline& rhs);

#pragma mark -

inline Guideline::Guideline(double x,
                            double y,
                            double angle,
                            const std::string& name,
                            const std::string& color,
                            const std::string& identifier)
    : x(x),
      y(y),
      angle(angle),
      name(name),
      color(color),
      identifier(identifier) {}

#pragma mark Comparison

inline bool operator==(const Guideline& lhs, const Guideline& rhs) {
  return (lhs.x == rhs.x &&
          lhs.y == rhs.y &&
          lhs.angle == rhs.angle &&
          lhs.name == rhs.name &&
          lhs.color == rhs.color &&
          lhs.identifier == rhs.identifier);
}

inline bool operator!=(const Guideline& lhs, const Guideline& rhs) {
  return !(lhs == rhs);
}

}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_GUIDELINE_H_
