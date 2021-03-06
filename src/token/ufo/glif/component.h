// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#pragma once
#ifndef TOKEN_UFO_GLIF_COMPONENT_H_
#define TOKEN_UFO_GLIF_COMPONENT_H_

#include <string>

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

  // Property tree
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

// MARK: -

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

// MARK: Comparison

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

// MARK: Property tree

inline Component::Component(const boost::property_tree::ptree& tree)
    : Component() {
  xml::readAttribute(tree, "base", &base);
  xml::readAttribute(tree, "xScale", &x_scale);
  xml::readAttribute(tree, "xyScale", &xy_scale);
  xml::readAttribute(tree, "yxScale", &yx_scale);
  xml::readAttribute(tree, "yScale", &y_scale);
  xml::readAttribute(tree, "xOffset", &x_offset);
  xml::readAttribute(tree, "yOffset", &y_offset);
  xml::readAttribute(tree, "identifier", &identifier);
}

inline boost::property_tree::ptree Component::ptree() const {
  boost::property_tree::ptree tree;
  xml::writeAttribute(&tree, "base", base);
  xml::writeAttribute(&tree, "xScale", x_scale, 1.0);
  xml::writeAttribute(&tree, "xyScale", xy_scale, 0.0);
  xml::writeAttribute(&tree, "yxScale", yx_scale, 0.0);
  xml::writeAttribute(&tree, "yScale", y_scale, 1.0);
  xml::writeAttribute(&tree, "xOffset", x_offset, 0.0);
  xml::writeAttribute(&tree, "yOffset", y_offset, 0.0);
  xml::writeAttribute(&tree, "identifier", identifier, "");
  return tree;
}

}  // namespace glif
}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_GLIF_COMPONENT_H_
