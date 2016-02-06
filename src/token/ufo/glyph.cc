//
//  token/ufo/glyph.cc
//
//  The MIT License
//
//  Copyright (C) 2015-2016 Shota Matsuda
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

#include "token/ufo/glyph.h"

#include <cassert>
#include <fstream>
#include <istream>
#include <ostream>
#include <string>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "token/ufo/xml.h"

namespace token {
namespace ufo {

#pragma mark Opening and saving

bool Glyph::open(const std::string& path) {
  std::ifstream stream(path);
  const auto result = open(&stream);
  stream.close();
  return result;
}

bool Glyph::open(std::istream *stream) {
  assert(stream);
  if (!stream->good()) {
    return false;
  }
  boost::property_tree::ptree tree;
  boost::property_tree::xml_parser::read_xml(*stream, tree);
  const auto& glyph = tree.get_child("glyph");
  xml::read_attr(glyph, "name", &name);
  xml::read_child(glyph, "advance", &advance);
  xml::read_children(glyph, "unicode", &unicodes);
  xml::read_child(glyph, "image", &image);
  xml::read_children(glyph, "guideline", &guidelines);
  xml::read_children(glyph, "anchor", &anchors);
  xml::read_child(glyph, "outline", &outline);
  xml::read_child(glyph, "lib", &lib);
  return true;
}

bool Glyph::save(const std::string& path) const {
  std::ofstream stream(path);
  const auto result = save(&stream);
  stream.close();
  return result;
}

bool Glyph::save(std::ostream *stream) const {
  assert(stream);
  if (!stream->good()) {
    return false;
  }
  boost::property_tree::ptree glyph;
  xml::write_attr(&glyph, "name", name);
  xml::write_attr(&glyph, "format", 2);
  xml::write_child(&glyph, "advance", advance);
  xml::write_children(&glyph, "unicode", unicodes);
  xml::write_child(&glyph, "image", image);
  xml::write_children(&glyph, "guideline", guidelines);
  xml::write_children(&glyph, "anchor", anchors);
  xml::write_child(&glyph, "outline", outline);
  xml::write_child(&glyph, "lib", lib);
  boost::property_tree::ptree tree;
  tree.add_child("glyph", glyph);
  boost::property_tree::xml_writer_settings<std::string> settings(' ', 2);
  boost::property_tree::xml_parser::write_xml(*stream, tree, settings);
  return true;
}

}  // namespace ufo
}  // namespace token
