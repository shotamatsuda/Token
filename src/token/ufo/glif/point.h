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
#ifndef TOKEN_UFO_GLIF_POINT_H_
#define TOKEN_UFO_GLIF_POINT_H_

#include <cassert>
#include <string>
#include <utility>

#include <boost/property_tree/ptree.hpp>

#include "token/ufo/xml.h"

namespace token {
namespace ufo {
namespace glif {

class Point final {
 public:
  enum class Type {
    MOVE,
    LINE,
    OFFCURVE,
    CURVE,
    QCURVE
  };

 public:
  Point();
  Point(double x,
        double y,
        Type type = Type::OFFCURVE,
        bool smooth = false,
        const std::string& name = std::string(),
        const std::string& identifier = std::string());

  // Copy semantics
  Point(const Point&) = default;
  Point& operator=(const Point&) = default;

  // Mutator
  void reset();

  // Concept: Property tree representable
  explicit Point(const boost::property_tree::ptree& tree);
  boost::property_tree::ptree ptree() const;

 public:
  double x;
  double y;
  Type type;
  bool smooth;
  std::string name;
  std::string identifier;
};

// Comparison
bool operator==(const Point& lhs, const Point& rhs);
bool operator!=(const Point& lhs, const Point& rhs);

// MARK: -

inline Point::Point() : type(Type::OFFCURVE), smooth() {}

inline Point::Point(double x,
                    double y,
                    Type type,
                    bool smooth,
                    const std::string& name,
                    const std::string& identifier)
    : x(x),
      y(y),
      type(type),
      smooth(smooth),
      name(name),
      identifier(identifier) {}

// MARK: Comparison

inline bool operator==(const Point& lhs, const Point& rhs) {
  return (lhs.x == rhs.x &&
          lhs.y == rhs.y &&
          lhs.type == rhs.type &&
          lhs.smooth == rhs.smooth &&
          lhs.name == rhs.name &&
          lhs.identifier == rhs.identifier);
}

inline bool operator!=(const Point& lhs, const Point& rhs) {
  return !(lhs == rhs);
}

// MARK: Property tree

inline Point::Point(const boost::property_tree::ptree& tree) : Point() {
  xml::readAttribute(tree, "x", &x);
  xml::readAttribute(tree, "y", &y);
  std::string type_string;
  xml::readAttribute(tree, "type", &type_string);
  if (type_string == "move") {
    type = Type::MOVE;
  } else if (type_string == "line") {
    type = Type::LINE;
  } else if (type_string == "offcurve") {
    type = Type::OFFCURVE;
  } else if (type_string == "curve") {
    type = Type::CURVE;
  } else if (type_string == "qcurve") {
    type = Type::QCURVE;
  }
  std::string smooth_string;
  xml::readAttribute(tree, "smooth", &smooth_string);
  if (smooth_string == "yes") {
    smooth = true;
  }
  xml::readAttribute(tree, "name", &name);
  xml::readAttribute(tree, "identifier", &identifier);
}

inline boost::property_tree::ptree Point::ptree() const {
  std::string type;
  switch (this->type) {
    case Type::MOVE: type = "move"; break;
    case Type::LINE: type = "line"; break;
    case Type::OFFCURVE: type = "offcurve"; break;
    case Type::CURVE: type = "curve"; break;
    case Type::QCURVE: type = "qcurve"; break;
    default:
      assert(false);
      break;
  }
  boost::property_tree::ptree tree;
  xml::writeAttribute(&tree, "x", x);
  xml::writeAttribute(&tree, "y", y);
  xml::writeAttribute(&tree, "type", type, "offcurve");
  xml::writeAttribute(&tree, "smooth", smooth ? "yes" : "no", "no");
  xml::writeAttribute(&tree, "name", name, "");
  xml::writeAttribute(&tree, "identifier", identifier, "");
  return tree;
}

}  // namespace glif
}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_GLIF_POINT_H_
