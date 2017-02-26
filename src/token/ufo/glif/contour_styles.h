//
//  The MIT License
//
//  Copyright (C) 2015-2017 Shota Matsuda
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
