//
//  token/ufo/glyph.h
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
#ifndef TOKEN_UFO_GLYPH_H_
#define TOKEN_UFO_GLYPH_H_

#include <string>
#include <utility>
#include <vector>

#include <boost/property_tree/ptree.hpp>

#include "token/ufo/advance.h"
#include "token/ufo/anchor.h"
#include "token/ufo/guideline.h"
#include "token/ufo/image.h"
#include "token/ufo/optional.h"
#include "token/ufo/outline.h"
#include "token/ufo/unicode.h"
#include "token/ufo/xml.h"

namespace token {
namespace ufo {

class Glyph final {
 public:
  Glyph() = default;
  explicit Glyph(const std::string& name);

  // Copy semantics
  Glyph(const Glyph&) = default;
  Glyph& operator=(const Glyph&) = default;

  // Comparison
  bool operator==(const Glyph& other) const;
  bool operator!=(const Glyph& other) const;

  // Property tree
  static Glyph read(const boost::property_tree::ptree& tree);
  boost::property_tree::ptree write() const;

 public:
  std::string name;
  Optional<Advance> advance;
  std::vector<Unicode> unicodes;
  Optional<Image> image;
  std::vector<Guideline> guidelines;
  std::vector<Anchor> anchors;
  Optional<Outline> outline;
};

#pragma mark -

inline Glyph::Glyph(const std::string& name) : name(name) {}

#pragma mark Comparison

inline bool Glyph::operator==(const Glyph& other) const {
  return (name == other.name &&
          advance == other.advance &&
          unicodes == other.unicodes &&
          image == other.image &&
          guidelines == other.guidelines &&
          anchors == other.anchors &&
          outline == other.outline);
}

inline bool Glyph::operator!=(const Glyph& other) const {
  return operator==(other);
}

#pragma mark Property tree

inline Glyph Glyph::read(const boost::property_tree::ptree& tree) {
  Glyph result;
  const auto& glyph = tree.get_child("glyph");
  xml::read_attr(glyph, "name", &result.name);
  xml::read_child(glyph, "advance", &result.advance);
  xml::read_children(glyph, "unicode", &result.unicodes);
  xml::read_child(glyph, "image", &result.image);
  xml::read_children(glyph, "guideline", &result.guidelines);
  xml::read_children(glyph, "anchor", &result.anchors);
  xml::read_child(glyph, "outline", &result.outline);
  return std::move(result);
}

inline boost::property_tree::ptree Glyph::write() const {
  boost::property_tree::ptree glyph;
  xml::write_attr(&glyph, "name", name);
  xml::write_attr(&glyph, "format", 2);
  xml::write_child(&glyph, "advance", advance);
  xml::write_children(&glyph, "unicode", unicodes);
  xml::write_child(&glyph, "image", image);
  xml::write_children(&glyph, "guideline", guidelines);
  xml::write_children(&glyph, "anchor", anchors);
  xml::write_child(&glyph, "outline", outline);
  boost::property_tree::ptree tree;
  tree.add_child("glyph", glyph);
  return std::move(tree);
}

}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_GLYPH_H_
