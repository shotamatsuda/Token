//
//  token/ufo/glif/contour.h
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
#ifndef TOKEN_UFO_GLIF_CONTOUR_H_
#define TOKEN_UFO_GLIF_CONTOUR_H_

#include <string>
#include <utility>
#include <vector>

#include <boost/property_tree/ptree.hpp>

#include "token/ufo/glif/point.h"
#include "token/ufo/xml.h"

namespace token {
namespace ufo {
namespace glif {

class Contour final {
 public:
  Contour() = default;

  // Copy semantics
  Contour(const Contour& other) = default;
  Contour& operator=(const Contour& other) = default;

  // Comparison
  bool operator==(const Contour& other) const;
  bool operator!=(const Contour& other) const;

  // Property tree
  static Contour read(const boost::property_tree::ptree& tree);
  boost::property_tree::ptree write() const;

 public:
  std::string identifier;
  std::vector<Point> points;
};

#pragma mark -

#pragma mark Comparison

inline bool Contour::operator==(const Contour& other) const {
  return (identifier == other.identifier && points == other.points);
}

inline bool Contour::operator!=(const Contour& other) const {
  return !operator==(other);
}

#pragma mark Property tree

inline Contour Contour::read(const boost::property_tree::ptree& tree) {
  Contour result;
  xml::read_attr(tree, "identifier", &result.identifier);
  xml::read_children(tree, "point", &result.points);
  return std::move(result);
}

inline boost::property_tree::ptree Contour::write() const {
  boost::property_tree::ptree tree;
  xml::write_attr(&tree, "identifier", identifier);
  xml::write_children(&tree, "point", points);
  return std::move(tree);
}

}  // namespace glif

using glif::Contour;

}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_GLIF_CONTOUR_H_
