//
//  token/ufo/glif/lib.h
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
#ifndef TOKEN_UFO_GLIF_LIB_H_
#define TOKEN_UFO_GLIF_LIB_H_

#include <utility>

#include <boost/property_tree/ptree.hpp>

#include "token/ufo/property_list.h"
#include "token/ufo/xml.h"

namespace token {
namespace ufo {
namespace glif {

class Lib final {
 public:
  Lib();
  explicit Lib(unsigned int number_of_contours);

  // Copy semantics
  Lib(const Lib&) = default;
  Lib& operator=(const Lib&) = default;

  // Comparison
  bool operator==(const Lib& other) const;
  bool operator!=(const Lib& other) const;

  // Property tree
  static Lib read(const boost::property_tree::ptree& tree);
  boost::property_tree::ptree ptree() const;

 private:
  static PropertyList convertToPropertyList(
      const boost::property_tree::ptree& tree);

 public:
  unsigned int number_of_contours;
};

#pragma mark -

inline Lib::Lib() : number_of_contours() {}

inline Lib::Lib(unsigned int number_of_contours)
    : number_of_contours(number_of_contours) {}

#pragma mark Comparison

inline bool Lib::operator==(const Lib& other) const {
  return number_of_contours == other.number_of_contours;
}

inline bool Lib::operator!=(const Lib& other) const {
  return !operator==(other);
}

}  // namespace glif
}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_GLIF_LIB_H_
