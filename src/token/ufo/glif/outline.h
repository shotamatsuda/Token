//
//  token/ufo/glif/outline.h
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
#ifndef TOKEN_UFO_GLIF_OUTLINE_H_
#define TOKEN_UFO_GLIF_OUTLINE_H_

#include <utility>
#include <vector>

#include <boost/property_tree/ptree.hpp>

#include "token/ufo/glif/component.h"
#include "token/ufo/glif/contour.h"
#include "token/ufo/xml.h"

namespace token {
namespace ufo {
namespace glif {

class Outline final {
 public:
  Outline() = default;
  Outline(const std::vector<Component>& components,
          const std::vector<Contour>& contours);

  // Copy semantics
  Outline(const Outline& other) = default;
  Outline& operator=(const Outline& other) = default;

  // Comparison
  bool operator==(const Outline& other) const;
  bool operator!=(const Outline& other) const;

  // Property tree
  static Outline read(const boost::property_tree::ptree& tree);
  boost::property_tree::ptree write() const;

 public:
  std::vector<Component> components;
  std::vector<Contour> contours;
};

#pragma mark -

inline Outline::Outline(const std::vector<Component>& components,
                        const std::vector<Contour>& contours)
    : components(components),
      contours(contours) {}

#pragma mark Comparison

inline bool Outline::operator==(const Outline& other) const {
  return (components == other.components && contours == other.contours);
}

inline bool Outline::operator!=(const Outline& other) const {
  return !operator==(other);
}

#pragma mark Property tree

inline Outline Outline::read(const boost::property_tree::ptree& tree) {
  Outline result;
  xml::read_children(tree, "component", &result.components);
  xml::read_children(tree, "contour", &result.contours);
  return std::move(result);
}

inline boost::property_tree::ptree Outline::write() const {
  boost::property_tree::ptree tree;
  xml::write_children(&tree, "component", components);
  xml::write_children(&tree, "contour", contours);
  return std::move(tree);
}

}  // namespace glif

using glif::Outline;

}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_GLIF_OUTLINE_H_
