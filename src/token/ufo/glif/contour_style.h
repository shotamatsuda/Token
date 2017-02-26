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
#ifndef TOKEN_UFO_GLIF_CONTOUR_STYLE_H_
#define TOKEN_UFO_GLIF_CONTOUR_STYLE_H_

#include <string>

#include <boost/property_tree/ptree.hpp>

#include "token/types.h"
#include "token/ufo/property_list.h"

namespace token {
namespace ufo {
namespace glif {

class ContourStyle final {
 public:
  ContourStyle();

  // Copy semantics
  ContourStyle(const ContourStyle&) = default;
  ContourStyle& operator=(const ContourStyle&) = default;

  // Property list
  explicit ContourStyle(const PropertyList& plist);
  PropertyList plist() const;

  // Property tree
  boost::property_tree::ptree ptree() const;

 public:
  Cap cap;
  Join join;
  Align align;
  bool filled;
};

// Comparison
bool operator==(const ContourStyle& lhs, const ContourStyle& rhs);
bool operator!=(const ContourStyle& lhs, const ContourStyle& rhs);

// MARK: -

inline ContourStyle::ContourStyle() : filled() {}

// MARK: Comparison

inline bool operator==(const ContourStyle& lhs, const ContourStyle& rhs) {
  return (lhs.cap == rhs.cap &&
          lhs.join == rhs.join &&
          lhs.align == rhs.align &&
          lhs.filled == rhs.filled);
}

inline bool operator!=(const ContourStyle& lhs, const ContourStyle& rhs) {
  return !(lhs == rhs);
}

}  // namespace glif
}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_GLIF_CONTOUR_STYLE_H_
