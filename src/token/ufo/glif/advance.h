// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#pragma once
#ifndef TOKEN_UFO_GLIF_ADVANCE_H_
#define TOKEN_UFO_GLIF_ADVANCE_H_

#include <boost/property_tree/ptree.hpp>

#include "token/ufo/xml.h"

namespace token {
namespace ufo {
namespace glif {

class Advance final {
 public:
  Advance();
  Advance(double width, double height);

  // Copy semantics
  Advance(const Advance&) = default;
  Advance& operator=(const Advance&) = default;

  // Property tree
  explicit Advance(const boost::property_tree::ptree& tree);
  boost::property_tree::ptree ptree() const;

 public:
  double width;
  double height;
};

// Comparison
bool operator==(const Advance& lhs, const Advance& rhs);
bool operator!=(const Advance& lhs, const Advance& rhs);

// MARK: -

inline Advance::Advance() : width(), height() {}

inline Advance::Advance(double width, double height)
    : width(width),
      height(height) {}

// MARK: Comparison

inline bool operator==(const Advance& lhs, const Advance& rhs) {
  return lhs.width == rhs.width && lhs.height == rhs.height;
}

inline bool operator!=(const Advance& lhs, const Advance& rhs) {
  return !(lhs == rhs);
}

// MARK: Property tree

inline Advance::Advance(const boost::property_tree::ptree& tree) : Advance() {
  xml::readAttribute(tree, "width", &width);
  xml::readAttribute(tree, "height", &height);
}

inline boost::property_tree::ptree Advance::ptree() const {
  boost::property_tree::ptree tree;
  xml::writeAttribute(&tree, "width", width, 0.0);
  xml::writeAttribute(&tree, "height", height, 0.0);
  return tree;
}

}  // namespace glif
}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_GLIF_ADVANCE_H_
