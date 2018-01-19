// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#pragma once
#ifndef TOKEN_UFO_GLIF_CONTOUR_STYLES_H_
#define TOKEN_UFO_GLIF_CONTOUR_STYLES_H_

#include <iterator>
#include <string>
#include <unordered_map>

#include <boost/property_tree/ptree.hpp>

#include "token/ufo/glif/contour_style.h"
#include "token/ufo/property_list.h"

namespace token {
namespace ufo {
namespace glif {

class ContourStyles final {
 public:
  ContourStyles() = default;

  // Copy semantics
  ContourStyles(const ContourStyles&) = default;
  ContourStyles& operator=(const ContourStyles&) = default;

  // Property list
  explicit ContourStyles(const PropertyList& plist);
  PropertyList plist() const;

  // Property tree
  boost::property_tree::ptree ptree() const;

  // Modifiers
  bool empty() const;
  const ContourStyle * find(const std::string& name) const;
  ContourStyle * find(const std::string& name);

  // Comparison
  friend bool operator==(const ContourStyles& lhs, const ContourStyles& rhs);
  friend bool operator!=(const ContourStyles& lhs, const ContourStyles& rhs);

 private:
  std::unordered_map<std::string, ContourStyle> styles_;
};

// MARK: -

// MARK: Comparison

inline bool operator==(const ContourStyles& lhs, const ContourStyles& rhs) {
  return lhs.styles_ == rhs.styles_;
}

inline bool operator!=(const ContourStyles& lhs, const ContourStyles& rhs) {
  return !(lhs == rhs);
}

// MARK: Modifiers

inline bool ContourStyles::empty() const {
  return styles_.empty();
}

inline const ContourStyle * ContourStyles::find(const std::string& name) const {
  const auto itr = styles_.find(name);
  if (itr == std::end(styles_)) {
    return nullptr;
  }
  return &itr->second;
}

inline ContourStyle * ContourStyles::find(const std::string& name) {
  const auto itr = styles_.find(name);
  if (itr == std::end(styles_)) {
    return nullptr;
  }
  return &itr->second;
}

}  // namespace glif
}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_GLIF_CONTOUR_STYLES_H_
