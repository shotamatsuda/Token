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
#include <utility>

#include <boost/property_tree/ptree.hpp>

#include "token/ufo/io.h"

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

  // Property tree
  static Guideline read(const boost::property_tree::ptree& tree);
  boost::property_tree::ptree write() const;

 public:
  double x;
  double y;
  double angle;
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

#pragma mark Property tree

inline Guideline Guideline::read(const boost::property_tree::ptree& tree) {
  Guideline result;
  io::read_attr(tree, "x", &result.x);
  io::read_attr(tree, "y", &result.y);
  io::read_attr(tree, "angle", &result.angle);
  io::read_attr(tree, "name", &result.name);
  io::read_attr(tree, "color", &result.color);
  io::read_attr(tree, "identifier", &result.identifier);
  return std::move(result);
}

inline boost::property_tree::ptree Guideline::write() const {
  boost::property_tree::ptree tree;
  io::write_attr(&tree, "x", x);
  io::write_attr(&tree, "y", y);
  io::write_attr(&tree, "angle", angle);
  io::write_attr(&tree, "name", name);
  io::write_attr(&tree, "color", color);
  io::write_attr(&tree, "identifier", identifier);
  return std::move(tree);
}

}  // namespace glif

using glif::Guideline;

}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_GLIF_GUIDELINE_H_
