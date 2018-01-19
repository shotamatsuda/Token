// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#pragma once
#ifndef TOKEN_UFO_GLIF_ANCHOR_H_
#define TOKEN_UFO_GLIF_ANCHOR_H_

#include <string>

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
  explicit Anchor(const boost::property_tree::ptree& tree);
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

// MARK: -

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

// MARK: Comparison

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

// MARK: Property tree

inline Anchor::Anchor(const boost::property_tree::ptree& tree) {
  xml::readAttribute(tree, "x", &x);
  xml::readAttribute(tree, "y", &y);
  xml::readAttribute(tree, "name", &name);
  xml::readAttribute(tree, "color", &color);
  xml::readAttribute(tree, "identifier", &identifier);
}

inline boost::property_tree::ptree Anchor::ptree() const {
  boost::property_tree::ptree tree;
  xml::writeAttribute(&tree, "x", x);
  xml::writeAttribute(&tree, "y", y);
  xml::writeAttribute(&tree, "name", name, "");
  xml::writeAttribute(&tree, "color", color, "");
  xml::writeAttribute(&tree, "identifier", identifier, "");
  return tree;
}

}  // namespace glif
}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_GLIF_ANCHOR_H_
