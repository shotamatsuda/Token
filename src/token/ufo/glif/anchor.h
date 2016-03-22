//
//  token/ufo/glif/anchor.h
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
#ifndef TOKEN_UFO_GLIF_ANCHOR_H_
#define TOKEN_UFO_GLIF_ANCHOR_H_

#include <string>
#include <utility>

#include <boost/property_tree/ptree.hpp>

#include "token/ufo/xml.h"

namespace token {
namespace ufo {
namespace glif {

class Anchor final {
 public:
  Anchor() = default;
  Anchor(double x,
         double y,
         const std::string& name,
         const std::string& color,
         const std::string& identifier);

  // Copy semantics
  Anchor(const Anchor&) = default;
  Anchor& operator=(const Anchor&) = default;

  // Property tree
  static Anchor read(const boost::property_tree::ptree& tree);
  boost::property_tree::ptree ptree() const;

 public:
  double x;
  double y;
  std::string name;
  std::string color;
  std::string identifier;
};

// Comparison
bool operator==(const Anchor& lhs, const Anchor& rhs);
bool operator!=(const Anchor& lhs, const Anchor& rhs);

#pragma mark -

inline Anchor::Anchor(double x,
                      double y,
                      const std::string& name,
                      const std::string& color,
                      const std::string& identifier)
    : x(x),
      y(y),
      name(name),
      color(color),
      identifier(identifier) {}

#pragma mark Comparison

inline bool operator==(const Anchor& lhs, const Anchor& rhs) {
  return (lhs.x == rhs.x &&
          lhs.y == rhs.y &&
          lhs.name == rhs.name &&
          lhs.color == rhs.color &&
          lhs.identifier == rhs.identifier);
}

inline bool operator!=(const Anchor& lhs, const Anchor& rhs) {
  return !(lhs == rhs);
}

#pragma mark Property tree

inline Anchor Anchor::read(const boost::property_tree::ptree& tree) {
  Anchor result;
  xml::read_attr(tree, "x", &result.x);
  xml::read_attr(tree, "y", &result.y);
  xml::read_attr(tree, "name", &result.name);
  xml::read_attr(tree, "color", &result.color);
  xml::read_attr(tree, "identifier", &result.identifier);
  return std::move(result);
}

inline boost::property_tree::ptree Anchor::ptree() const {
  boost::property_tree::ptree tree;
  xml::write_attr(&tree, "x", x);
  xml::write_attr(&tree, "y", y);
  xml::write_attr(&tree, "name", name, "");
  xml::write_attr(&tree, "color", color, "");
  xml::write_attr(&tree, "identifier", identifier, "");
  return std::move(tree);
}

}  // namespace glif
}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_GLIF_ANCHOR_H_
