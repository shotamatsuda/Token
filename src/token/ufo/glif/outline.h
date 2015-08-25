//
//  token/ufo/glif/outline.h
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
#ifndef TOKEN_UFO_GLIF_OUTLINE_H_
#define TOKEN_UFO_GLIF_OUTLINE_H_

#include <algorithm>
#include <memory>
#include <utility>
#include <vector>

#include <boost/property_tree/ptree.hpp>

#include "token/ufo/glif/component.h"
#include "token/ufo/glif/contour.h"
#include "token/ufo/io.h"

namespace token {
namespace ufo {
namespace glif {

class Outline final {
 public:
  Outline() = default;

  // Disallow copy semantics
  Outline(const Outline& other) = delete;
  Outline& operator=(const Outline& other) = delete;

  // Move semantics
  Outline(Outline&& other) = default;
  Outline& operator=(Outline&& other) = default;

  // Comparison
  bool operator==(const Outline& other) const;
  bool operator!=(const Outline& other) const;

  // Property tree
  static std::unique_ptr<Outline> read(
      const boost::property_tree::ptree& tree);
  boost::property_tree::ptree write() const;

 public:
  std::vector<std::unique_ptr<Component>> components;
  std::vector<std::unique_ptr<Contour>> contours;
};

#pragma mark -

#pragma mark Comparison

inline bool Outline::operator==(const Outline& other) const {
  const auto predicate = [](const auto& a, const auto& b) { return *a == *b; };
  return (std::equal(std::begin(components), std::end(components),
                     std::begin(other.components), predicate) &&
          std::equal(std::begin(contours), std::end(contours),
                     std::begin(other.contours), predicate));
}

inline bool Outline::operator!=(const Outline& other) const {
  return !operator==(other);
}

#pragma mark Property tree

inline std::unique_ptr<Outline> Outline::read(
    const boost::property_tree::ptree& tree) {
  auto result = std::make_unique<Outline>();
  io::read_children(tree, "component", &result->components);
  io::read_children(tree, "contour", &result->contours);
  return std::move(result);
}

inline boost::property_tree::ptree Outline::write() const {
  boost::property_tree::ptree tree;
  io::write_children(&tree, "component", components);
  io::write_children(&tree, "contour", contours);
  return std::move(tree);
}

}  // namespace glif

using glif::Outline;

}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_GLIF_OUTLINE_H_
