//
//  token/ufo/unicode.h
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
#ifndef TOKEN_UFO_UNICODE_H_
#define TOKEN_UFO_UNICODE_H_

#include <string>
#include <utility>

#include <boost/property_tree/ptree.hpp>

#include "token/ufo/xml.h"

namespace token {
namespace ufo {

class Unicode final {
 public:
  Unicode() = default;
  explicit Unicode(const std::string& hex);

  // Copy semantics
  Unicode(const Unicode& other) = default;
  Unicode& operator=(const Unicode& other) = default;

  // Comparison
  bool operator==(const Unicode& other) const;
  bool operator!=(const Unicode& other) const;

  // Property tree
  static Unicode read(const boost::property_tree::ptree& tree);
  boost::property_tree::ptree write() const;

 public:
  std::string hex;
};

#pragma mark -

inline Unicode::Unicode(const std::string& hex) : hex(hex) {}

#pragma mark Comparison

inline bool Unicode::operator==(const Unicode& other) const {
  return hex == other.hex;
}

inline bool Unicode::operator!=(const Unicode& other) const {
  return !operator==(other);
}

#pragma mark Property tree

inline Unicode Unicode::read(const boost::property_tree::ptree& tree) {
  Unicode result;
  xml::read_attr(tree, "hex", &result.hex);
  return std::move(result);
}

inline boost::property_tree::ptree Unicode::write() const {
  boost::property_tree::ptree tree;
  xml::write_attr(&tree, "hex", hex);
  return std::move(tree);
}

}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_UNICODE_H_
