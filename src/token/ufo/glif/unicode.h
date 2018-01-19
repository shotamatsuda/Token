// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#pragma once
#ifndef TOKEN_UFO_GLIF_UNICODE_H_
#define TOKEN_UFO_GLIF_UNICODE_H_

#include <string>

#include <boost/property_tree/ptree.hpp>

#include "token/ufo/xml.h"

namespace token {
namespace ufo {
namespace glif {

class Unicode final {
 public:
  Unicode() = default;
  explicit Unicode(const std::string& hex);

  // Copy semantics
  Unicode(const Unicode&) = default;
  Unicode& operator=(const Unicode&) = default;

  // Property tree
  explicit Unicode(const boost::property_tree::ptree& tree);
  boost::property_tree::ptree ptree() const;

 public:
  std::string hex;
};

// Comparison
bool operator==(const Unicode& lhs, const Unicode& rhs);
bool operator!=(const Unicode& lhs, const Unicode& rhs);

// MARK: -

inline Unicode::Unicode(const std::string& hex) : hex(hex) {}

// MARK: Comparison

inline bool operator==(const Unicode& lhs, const Unicode& rhs) {
  return lhs.hex == rhs.hex;
}

inline bool operator!=(const Unicode& lhs, const Unicode& rhs) {
  return !(lhs == rhs);
}

// MARK: Property tree

inline Unicode::Unicode(const boost::property_tree::ptree& tree) {
  xml::readAttribute(tree, "hex", &hex);
}

inline boost::property_tree::ptree Unicode::ptree() const {
  boost::property_tree::ptree tree;
  xml::writeAttribute(&tree, "hex", hex);
  return tree;
}

}  // namespace glif
}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_GLIF_UNICODE_H_
