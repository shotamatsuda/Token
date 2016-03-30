//
//  token/ufo/glif/component.h
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
#ifndef TOKEN_UFO_GLIF_COMPONENT_H_
#define TOKEN_UFO_GLIF_COMPONENT_H_

#include <string>
#include <utility>

#include <boost/property_tree/ptree.hpp>

#include "token/ufo/xml.h"

namespace token {
namespace ufo {
namespace glif {

class Component final {
 public:
  Component();
  Component(const std::string& base,
            double x_scale,
            double xy_scale,
            double yx_scale,
            double y_scale,
            double x_offset,
            double y_offset,
            const std::string& identifier);

  // Copy semantics
  Component(const Component&) = default;
  Component& operator=(const Component&) = default;

  // Concept: Property tree representable
  explicit Component(const boost::property_tree::ptree& tree);
  boost::property_tree::ptree ptree() const;

 public:
  std::string base;
  double x_scale;
  double xy_scale;
  double yx_scale;
  double y_scale;
  double x_offset;
  double y_offset;
  std::string identifier;
};

// Comparison
bool operator==(const Component& lhs, const Component& rhs);
bool operator!=(const Component& lhs, const Component& rhs);

#pragma mark -

inline Component::Component()
    : x_scale(1.0),
      xy_scale(),
      yx_scale(),
      y_scale(1.0),
      x_offset(),
      y_offset() {}

inline Component::Component(const std::string& base,
                            double x_scale,
                            double xy_scale,
                            double yx_scale,
                            double y_scale,
                            double x_offset,
                            double y_offset,
                            const std::string& identifier)
    : base(base),
      x_scale(x_scale),
      xy_scale(xy_scale),
      yx_scale(yx_scale),
      y_scale(y_scale),
      x_offset(x_offset),
      y_offset(y_offset),
      identifier(identifier) {}

#pragma mark Comparison

inline bool operator==(const Component& lhs, const Component& rhs) {
  return (lhs.base == rhs.base &&
          lhs.x_scale == rhs.x_scale &&
          lhs.xy_scale == rhs.xy_scale &&
          lhs.yx_scale == rhs.yx_scale &&
          lhs.y_scale == rhs.y_scale &&
          lhs.x_offset == rhs.x_offset &&
          lhs.y_offset == rhs.y_offset &&
          lhs.identifier == rhs.identifier);
}

inline bool operator!=(const Component& lhs, const Component& rhs) {
  return !(lhs == rhs);
}

#pragma mark Property tree

inline Component::Component(const boost::property_tree::ptree& tree) {
  xml::read_attribute(tree, "base", &base);
  xml::read_attribute(tree, "xScale", &x_scale);
  xml::read_attribute(tree, "xyScale", &xy_scale);
  xml::read_attribute(tree, "yxScale", &yx_scale);
  xml::read_attribute(tree, "yScale", &y_scale);
  xml::read_attribute(tree, "xOffset", &x_offset);
  xml::read_attribute(tree, "yOffset", &y_offset);
  xml::read_attribute(tree, "identifier", &identifier);
}

inline boost::property_tree::ptree Component::ptree() const {
  boost::property_tree::ptree tree;
  xml::write_attribute(&tree, "base", base);
  xml::write_attribute(&tree, "xScale", x_scale, 1.0);
  xml::write_attribute(&tree, "xyScale", xy_scale, 0.0);
  xml::write_attribute(&tree, "yxScale", yx_scale, 0.0);
  xml::write_attribute(&tree, "yScale", y_scale, 1.0);
  xml::write_attribute(&tree, "xOffset", x_offset, 0.0);
  xml::write_attribute(&tree, "yOffset", y_offset, 0.0);
  xml::write_attribute(&tree, "identifier", identifier, "");
  return std::move(tree);
}

}  // namespace glif
}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_GLIF_COMPONENT_H_
