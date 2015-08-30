//
//  token/ufo/point.h
//
//  The MIT License
//
//  Copyright (C) 2015 Shota Matsuda
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
#ifndef TOKEN_UFO_POINT_H_
#define TOKEN_UFO_POINT_H_

#include <cassert>
#include <string>
#include <utility>

#include <boost/property_tree/ptree.hpp>

#include "token/ufo/xml.h"

namespace token {
namespace ufo {

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

  // Comparison
  bool operator==(const Point& other) const;
  bool operator!=(const Point& other) const;

  // Mutator
  void reset();

  // Property tree
  static Point read(const boost::property_tree::ptree& tree);
  boost::property_tree::ptree write() const;

 public:
  double x;
  double y;
  Type type;
  bool smooth;
  std::string name;
  std::string identifier;
};

#pragma mark -

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

#pragma mark Comparison

inline bool Point::operator==(const Point& other) const {
  return (x == other.x &&
          y == other.y &&
          type == other.type &&
          smooth == other.smooth &&
          name == other.name &&
          identifier == other.identifier);
}

inline bool Point::operator!=(const Point& other) const {
  return !operator==(other);
}

#pragma mark Property tree

inline Point Point::read(const boost::property_tree::ptree& tree) {
  Point result;
  xml::read_attr(tree, "x", &result.x);
  xml::read_attr(tree, "y", &result.y);
  std::string type;
  xml::read_attr(tree, "type", &type);
  if (type == "move") {
    result.type = Type::MOVE;
  } else if (type == "line") {
    result.type = Type::LINE;
  } else if (type == "offcurve") {
    result.type = Type::OFFCURVE;
  } else if (type == "curve") {
    result.type = Type::CURVE;
  } else if (type == "qcurve") {
    result.type = Type::QCURVE;
  }
  std::string smooth;
  xml::read_attr(tree, "smooth", &smooth);
  if (smooth == "yes") {
    result.smooth = true;
  }
  xml::read_attr(tree, "name", &result.name);
  xml::read_attr(tree, "identifier", &result.identifier);
  return std::move(result);
}

inline boost::property_tree::ptree Point::write() const {
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
  xml::write_attr(&tree, "x", x);
  xml::write_attr(&tree, "y", y);
  xml::write_attr(&tree, "type", type, "offcurve");
  xml::write_attr(&tree, "smooth", smooth ? "yes" : "no", "no");
  xml::write_attr(&tree, "name", name, "");
  xml::write_attr(&tree, "identifier", identifier, "");
  return std::move(tree);
}

}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_POINT_H_
