// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#pragma once
#ifndef TOKEN_UFO_GLIF_LIB_H_
#define TOKEN_UFO_GLIF_LIB_H_

#include <boost/property_tree/ptree.hpp>

#include "token/ufo/glif/contour_styles.h"
#include "token/ufo/property_list.h"
#include "token/ufo/xml.h"

namespace token {
namespace ufo {
namespace glif {

class Lib final {
 public:
  Lib();
  Lib(unsigned int number_of_contours,
      unsigned int number_of_holes,
      const ContourStyles& contour_styles);

  // Copy semantics
  Lib(const Lib&) = default;
  Lib& operator=(const Lib&) = default;

  // Property tree
  explicit Lib(const boost::property_tree::ptree& tree);
  boost::property_tree::ptree ptree() const;

 private:
  static PropertyList convertToPropertyList(
      const boost::property_tree::ptree& tree);

 public:
  unsigned int number_of_contours;
  unsigned int number_of_holes;
  ContourStyles contour_styles;
};

// Comparison
bool operator==(const Lib& lhs, const Lib& rhs);
bool operator!=(const Lib& lhs, const Lib& rhs);

// MARK: -

inline Lib::Lib() : number_of_contours(), number_of_holes() {}

inline Lib::Lib(unsigned int number_of_contours,
                unsigned int number_of_holes,
                const ContourStyles& contour_styles)
    : number_of_contours(number_of_contours),
      number_of_holes(number_of_holes),
      contour_styles(contour_styles) {}

// MARK: Comparison

inline bool operator==(const Lib& lhs, const Lib& rhs) {
  return (lhs.number_of_contours == rhs.number_of_contours &&
          lhs.number_of_holes == rhs.number_of_holes &&
          lhs.contour_styles == rhs.contour_styles);
}

inline bool operator!=(const Lib& lhs, const Lib& rhs) {
  return !(lhs == rhs);
}

}  // namespace glif
}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_GLIF_LIB_H_
