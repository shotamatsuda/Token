//
//  token/ufo/glif/image.h
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
#ifndef TOKEN_UFO_GLIF_IMAGE_H_
#define TOKEN_UFO_GLIF_IMAGE_H_

#include <memory>
#include <string>
#include <utility>

#include <boost/property_tree/ptree.hpp>

#include "token/ufo/io.h"

namespace token {
namespace ufo {
namespace glif {

class Image final {
 public:
  Image();

  // Copy semantics
  Image(const Image& other) = default;
  Image& operator=(const Image& other) = default;

  // Comparison
  bool operator==(const Image& other) const;
  bool operator!=(const Image& other) const;

  // Property tree
  static std::unique_ptr<Image> read(
      const boost::property_tree::ptree& tree);
  boost::property_tree::ptree write() const;

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

#pragma mark -

inline Image::Image()
    : x_scale(1.0),
      xy_scale(),
      yx_scale(),
      y_scale(1.0),
      x_offset(),
      y_offset() {}

#pragma mark Comparison

inline bool Image::operator==(const Image& other) const {
  return (file_name == other.file_name &&
          x_scale == other.x_scale &&
          xy_scale == other.xy_scale &&
          yx_scale == other.yx_scale &&
          y_scale == other.y_scale &&
          x_offset == other.x_offset &&
          y_offset == other.y_offset &&
          color == other.color);
}

inline bool Image::operator!=(const Image& other) const {
  return !operator==(other);
}

#pragma mark Property tree

inline std::unique_ptr<Image> Image::read(
    const boost::property_tree::ptree& tree) {
  auto result = std::make_unique<Image>();
  io::read_attr(tree, "fileName", &result->file_name);
  io::read_attr(tree, "xScale", &result->x_scale);
  io::read_attr(tree, "xyScale", &result->xy_scale);
  io::read_attr(tree, "yxScale", &result->yx_scale);
  io::read_attr(tree, "yScale", &result->y_scale);
  io::read_attr(tree, "xOffset", &result->x_offset);
  io::read_attr(tree, "yOffset", &result->y_offset);
  io::read_attr(tree, "color", &result->color);
  return std::move(result);
}

inline boost::property_tree::ptree Image::write() const {
  boost::property_tree::ptree tree;
  io::write_attr(&tree, "fileName", file_name);
  io::write_attr(&tree, "xScale", x_scale);
  io::write_attr(&tree, "xyScale", xy_scale);
  io::write_attr(&tree, "yxScale", yx_scale);
  io::write_attr(&tree, "yScale", y_scale);
  io::write_attr(&tree, "xOffset", x_offset);
  io::write_attr(&tree, "yOffset", y_offset);
  io::write_attr(&tree, "color", color);
  return std::move(tree);
}

}  // namespace glif

using glif::Image;

}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_GLIF_IMAGE_H_
