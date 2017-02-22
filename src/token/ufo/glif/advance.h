//
//  token/ufo/glif/advance.h
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
#ifndef TOKEN_UFO_GLIF_ADVANCE_H_
#define TOKEN_UFO_GLIF_ADVANCE_H_

#include <utility>

#include <boost/property_tree/ptree.hpp>

#include "token/ufo/xml.h"

namespace token {
namespace ufo {
namespace glif {

class Advance final {
 public:
  Advance();
  Advance(double width, double height);

  // Copy semantics
  Advance(const Advance&) = default;
  Advance& operator=(const Advance&) = default;

  // Concept: Property tree representable
  explicit Advance(const boost::property_tree::ptree& tree);
  boost::property_tree::ptree ptree() const;

 public:
  double width;
  double height;
};

// Comparison
bool operator==(const Advance& lhs, const Advance& rhs);
bool operator!=(const Advance& lhs, const Advance& rhs);

// MARK: -

inline Advance::Advance() : width(), height() {}

inline Advance::Advance(double width, double height)
    : width(width),
      height(height) {}

// MARK: Comparison

inline bool operator==(const Advance& lhs, const Advance& rhs) {
  return lhs.width == rhs.width && lhs.height == rhs.height;
}

inline bool operator!=(const Advance& lhs, const Advance& rhs) {
  return !(lhs == rhs);
}

// MARK: Property tree

inline Advance::Advance(const boost::property_tree::ptree& tree) : Advance() {
  xml::read_attribute(tree, "width", &width);
  xml::read_attribute(tree, "height", &height);
}

inline boost::property_tree::ptree Advance::ptree() const {
  boost::property_tree::ptree tree;
  xml::write_attribute(&tree, "width", width, 0.0);
  xml::write_attribute(&tree, "height", height, 0.0);
  return std::move(tree);
}

}  // namespace glif
}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_GLIF_ADVANCE_H_
