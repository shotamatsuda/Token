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
#ifndef TOKEN_UFO_GLIF_IMAGE_H_
#define TOKEN_UFO_GLIF_IMAGE_H_

#include <string>
#include <utility>

#include <boost/property_tree/ptree.hpp>

#include "token/ufo/xml.h"

namespace token {
namespace ufo {
namespace glif {

class Image final {
 public:
  Image();
  Image(const std::string& file_name,
        double x_scale,
        double xy_scale,
        double yx_scale,
        double y_scale,
        double x_offset,
        double y_offset,
        const std::string& color);

  // Copy semantics
  Image(const Image&) = default;
  Image& operator=(const Image&) = default;

  // Concept: Property tree representable
  explicit Image(const boost::property_tree::ptree& tree);
  boost::property_tree::ptree ptree() const;

 public:
  std::string file_name;
  double x_scale;
  double xy_scale;
  double yx_scale;
  double y_scale;
  double x_offset;
  double y_offset;
  std::string color;
};

// Comparison
bool operator==(const Image& lhs, const Image& rhs);
bool operator!=(const Image& lhs, const Image& rhs);

// MARK: -

inline Image::Image()
    : x_scale(1.0),
      xy_scale(),
      yx_scale(),
      y_scale(1.0),
      x_offset(),
      y_offset() {}

inline Image::Image(const std::string& file_name,
                    double x_scale,
                    double xy_scale,
                    double yx_scale,
                    double y_scale,
                    double x_offset,
                    double y_offset,
                    const std::string& color)
    : file_name(file_name),
      x_scale(x_scale),
      xy_scale(xy_scale),
      yx_scale(yx_scale),
      y_scale(y_scale),
      x_offset(x_offset),
      y_offset(y_offset),
      color(color) {}

// MARK: Comparison

inline bool operator==(const Image& lhs, const Image& rhs) {
  return (lhs.file_name == rhs.file_name &&
          lhs.x_scale == rhs.x_scale &&
          lhs.xy_scale == rhs.xy_scale &&
          lhs.yx_scale == rhs.yx_scale &&
          lhs.y_scale == rhs.y_scale &&
          lhs.x_offset == rhs.x_offset &&
          lhs.y_offset == rhs.y_offset &&
          lhs.color == rhs.color);
}

inline bool operator!=(const Image& lhs, const Image& rhs) {
  return !(lhs == rhs);
}

// MARK: Property tree

inline Image::Image(const boost::property_tree::ptree& tree) : Image() {
  xml::readAttribute(tree, "fileName", &file_name);
  xml::readAttribute(tree, "xScale", &x_scale);
  xml::readAttribute(tree, "xyScale", &xy_scale);
  xml::readAttribute(tree, "yxScale", &yx_scale);
  xml::readAttribute(tree, "yScale", &y_scale);
  xml::readAttribute(tree, "xOffset", &x_offset);
  xml::readAttribute(tree, "yOffset", &y_offset);
  xml::readAttribute(tree, "color", &color);
}

inline boost::property_tree::ptree Image::ptree() const {
  boost::property_tree::ptree tree;
  xml::writeAttribute(&tree, "fileName", file_name);
  xml::writeAttribute(&tree, "xScale", x_scale, 1.0);
  xml::writeAttribute(&tree, "xyScale", xy_scale, 0.0);
  xml::writeAttribute(&tree, "yxScale", yx_scale, 0.0);
  xml::writeAttribute(&tree, "yScale", y_scale, 1.0);
  xml::writeAttribute(&tree, "xOffset", x_offset, 0.0);
  xml::writeAttribute(&tree, "yOffset", y_offset, 0.0);
  xml::writeAttribute(&tree, "color", color, "");
  return tree;
}

}  // namespace glif
}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_GLIF_IMAGE_H_
