// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#pragma once
#ifndef TOKEN_UFO_GLIF_CONTOUR_H_
#define TOKEN_UFO_GLIF_CONTOUR_H_

#include <string>
#include <vector>

#include <boost/property_tree/ptree.hpp>

#include "token/ufo/glif/point.h"
#include "token/ufo/xml.h"

namespace token {
namespace ufo {
namespace glif {

class Contour final {
 public:
  Contour() = default;
  Contour(const std::string& identifier,
          const std::vector<Point>& points);

  // Copy semantics
  Contour(const Contour&) = default;
  Contour& operator=(const Contour&) = default;

  // Property tree
  explicit Contour(const boost::property_tree::ptree& tree);
  boost::property_tree::ptree ptree() const;

 public:
  std::string identifier;
  std::vector<Point> points;
};

// Comparison
bool operator==(const Contour& lhs, const Contour& rhs);
bool operator!=(const Contour& lhs, const Contour& rhs);

// MARK: -

inline Contour::Contour(const std::string& identifier,
                        const std::vector<Point>& points)
    : identifier(identifier),
      points(points) {}

// MARK: Comparison

inline bool operator==(const Contour& lhs, const Contour& rhs) {
  return lhs.identifier == rhs.identifier && lhs.points == rhs.points;
}

inline bool operator!=(const Contour& lhs, const Contour& rhs) {
  return !(lhs == rhs);
}

// MARK: Property tree

inline Contour::Contour(const boost::property_tree::ptree& tree) {
  xml::readAttribute(tree, "identifier", &identifier);
  xml::readChildren(tree, "point", &points);
}

inline boost::property_tree::ptree Contour::ptree() const {
  boost::property_tree::ptree tree;
  xml::writeAttribute(&tree, "identifier", identifier, "");
  xml::writeChildren(&tree, "point", points);
  return tree;
}

}  // namespace glif
}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_GLIF_CONTOUR_H_
