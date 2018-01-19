// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#pragma once
#ifndef TOKEN_UFO_GLIF_GUIDELINE_H_
#define TOKEN_UFO_GLIF_GUIDELINE_H_

#include <string>

#include <boost/property_tree/ptree.hpp>

#include "token/ufo/xml.h"

namespace token {
namespace ufo {
namespace glif {

class Guideline final {
 public:
  Guideline() = default;
  Guideline(double x,
            double y,
            double angle,
            const std::string& name,
            const std::string& color,
            const std::string& identifier);

  // Copy semantics
  Guideline(const Guideline&) = default;
  Guideline& operator=(const Guideline&) = default;

  // Property tree
  explicit Guideline(const boost::property_tree::ptree& tree);
  boost::property_tree::ptree ptree() const;

 public:
  double x;
  double y;
  double angle;
  std::string name;
  std::string color;
  std::string identifier;
};

// Comparison
bool operator==(const Guideline& lhs, const Guideline& rhs);
bool operator!=(const Guideline& lhs, const Guideline& rhs);

// MARK: -

inline Guideline::Guideline(double x,
                            double y,
                            double angle,
                            const std::string& name,
                            const std::string& color,
                            const std::string& identifier)
    : x(x),
      y(y),
      angle(angle),
      name(name),
      color(color),
      identifier(identifier) {}

// MARK: Comparison

inline bool operator==(const Guideline& lhs, const Guideline& rhs) {
  return (lhs.x == rhs.x &&
          lhs.y == rhs.y &&
          lhs.angle == rhs.angle &&
          lhs.name == rhs.name &&
          lhs.color == rhs.color &&
          lhs.identifier == rhs.identifier);
}

inline bool operator!=(const Guideline& lhs, const Guideline& rhs) {
  return !(lhs == rhs);
}

// MARK: Property tree

inline Guideline::Guideline(const boost::property_tree::ptree& tree) {
  xml::readAttribute(tree, "x", &x);
  xml::readAttribute(tree, "y", &y);
  xml::readAttribute(tree, "angle", &angle);
  xml::readAttribute(tree, "name", &name);
  xml::readAttribute(tree, "color", &color);
  xml::readAttribute(tree, "identifier", &identifier);
}

inline boost::property_tree::ptree Guideline::ptree() const {
  boost::property_tree::ptree tree;
  xml::writeAttribute(&tree, "x", x);
  xml::writeAttribute(&tree, "y", y);
  xml::writeAttribute(&tree, "angle", angle);
  xml::writeAttribute(&tree, "name", name, "");
  xml::writeAttribute(&tree, "color", color, "");
  xml::writeAttribute(&tree, "identifier", identifier, "");
  return tree;
}

}  // namespace glif
}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_GLIF_GUIDELINE_H_
