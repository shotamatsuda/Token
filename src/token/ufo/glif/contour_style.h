// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#pragma once
#ifndef TOKEN_UFO_GLIF_CONTOUR_STYLE_H_
#define TOKEN_UFO_GLIF_CONTOUR_STYLE_H_

#include <string>

#include <boost/property_tree/ptree.hpp>

#include "token/types.h"
#include "token/ufo/property_list.h"

namespace token {
namespace ufo {
namespace glif {

class ContourStyle final {
 public:
  ContourStyle();

  // Copy semantics
  ContourStyle(const ContourStyle&) = default;
  ContourStyle& operator=(const ContourStyle&) = default;

  // Property list
  explicit ContourStyle(const PropertyList& plist);
  PropertyList plist() const;

  // Property tree
  boost::property_tree::ptree ptree() const;

 public:
  Cap cap;
  Join join;
  Align align;
  bool filled;
};

// Comparison
bool operator==(const ContourStyle& lhs, const ContourStyle& rhs);
bool operator!=(const ContourStyle& lhs, const ContourStyle& rhs);

// MARK: -

inline ContourStyle::ContourStyle() : filled() {}

// MARK: Comparison

inline bool operator==(const ContourStyle& lhs, const ContourStyle& rhs) {
  return (lhs.cap == rhs.cap &&
          lhs.join == rhs.join &&
          lhs.align == rhs.align &&
          lhs.filled == rhs.filled);
}

inline bool operator!=(const ContourStyle& lhs, const ContourStyle& rhs) {
  return !(lhs == rhs);
}

}  // namespace glif
}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_GLIF_CONTOUR_STYLE_H_
