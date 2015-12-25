//
//  token/ufo/glif/unicode.h
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
#ifndef TOKEN_UFO_GLIF_UNICODE_H_
#define TOKEN_UFO_GLIF_UNICODE_H_

#include <string>
#include <utility>

#include <boost/property_tree/ptree.hpp>

#include "token/ufo/xml.h"

namespace token {
namespace ufo {
namespace glif {

class Unicode final {
 public:
  Unicode() = default;
  explicit Unicode(const std::string& hex);

  // Copy semantics
  Unicode(const Unicode&) = default;
  Unicode& operator=(const Unicode&) = default;

  // Property tree
  static Unicode read(const boost::property_tree::ptree& tree);
  boost::property_tree::ptree ptree() const;

 public:
  std::string hex;
};

// Comparison
bool operator==(const Unicode& lhs, const Unicode& rhs);
bool operator!=(const Unicode& lhs, const Unicode& rhs);

#pragma mark -

inline Unicode::Unicode(const std::string& hex) : hex(hex) {}

#pragma mark Comparison

inline bool operator==(const Unicode& lhs, const Unicode& rhs) {
  return lhs.hex == rhs.hex;
}

inline bool operator!=(const Unicode& lhs, const Unicode& rhs) {
  return !(lhs == rhs);
}

#pragma mark Property tree

inline Unicode Unicode::read(const boost::property_tree::ptree& tree) {
  Unicode result;
  xml::read_attr(tree, "hex", &result.hex);
  return std::move(result);
}

inline boost::property_tree::ptree Unicode::ptree() const {
  boost::property_tree::ptree tree;
  xml::write_attr(&tree, "hex", hex);
  return std::move(tree);
}

}  // namespace glif
}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_GLIF_UNICODE_H_
