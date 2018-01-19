// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#pragma once
#ifndef TOKEN_UFO_GLIF_OUTLINE_H_
#define TOKEN_UFO_GLIF_OUTLINE_H_

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
  Outline(const Outline&) = default;
  Outline& operator=(const Outline&) = default;

  // Property tree
  explicit Outline(const boost::property_tree::ptree& tree);
  boost::property_tree::ptree ptree() const;

 public:
  std::vector<Component> components;
  std::vector<Contour> contours;
};

// Comparison
bool operator==(const Outline& lhs, const Outline& rhs);
bool operator!=(const Outline& lhs, const Outline& rhs);

// MARK: -

inline Outline::Outline(const std::vector<Component>& components,
                        const std::vector<Contour>& contours)
    : components(components),
      contours(contours) {}

// MARK: Comparison

inline bool operator==(const Outline& lhs, const Outline& rhs) {
  return lhs.components == rhs.components && lhs.contours == rhs.contours;
}

inline bool operator!=(const Outline& lhs, const Outline& rhs) {
  return !(lhs == rhs);
}

// MARK: Property tree

inline Outline::Outline(const boost::property_tree::ptree& tree) {
  xml::readChildren(tree, "component", &components);
  xml::readChildren(tree, "contour", &contours);
}

inline boost::property_tree::ptree Outline::ptree() const {
  boost::property_tree::ptree tree;
  xml::writeChildren(&tree, "component", components);
  xml::writeChildren(&tree, "contour", contours);
  return tree;
}

}  // namespace glif
}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_GLIF_OUTLINE_H_
