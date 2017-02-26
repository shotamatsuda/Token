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

#include "token/ufo/glyph.h"

#include <fstream>
#include <istream>
#include <ostream>
#include <string>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "token/ufo/xml.h"

namespace token {
namespace ufo {

// MARK: Opening and saving

bool Glyph::open(const std::string& path) {
  std::ifstream stream(path);
  const auto result = open(stream);
  stream.close();
  return result;
}

bool Glyph::open(std::istream& stream) {
  if (!stream.good()) {
    return false;
  }
  boost::property_tree::ptree tree;
  boost::property_tree::xml_parser::read_xml(stream, tree);
  const auto& glyph = tree.get_child("glyph");
  xml::readAttribute(glyph, "name", &name);
  xml::readChild(glyph, "advance", &advance);
  xml::readChildren(glyph, "unicode", &unicodes);
  xml::readChild(glyph, "image", &image);
  xml::readChildren(glyph, "guideline", &guidelines);
  xml::readChildren(glyph, "anchor", &anchors);
  xml::readChild(glyph, "outline", &outline);
  xml::readChild(glyph, "lib", &lib);
  return true;
}

bool Glyph::save(const std::string& path) const {
  std::ofstream stream(path);
  const auto result = save(stream);
  stream.close();
  return result;
}

bool Glyph::save(std::ostream& stream) const {
  if (!stream.good()) {
    return false;
  }
  boost::property_tree::ptree glyph;
  xml::writeAttribute(&glyph, "name", name);
  xml::writeAttribute(&glyph, "format", 2);
  xml::writeChild(&glyph, "advance", advance);
  xml::writeChildren(&glyph, "unicode", unicodes);
  xml::writeChild(&glyph, "image", image);
  xml::writeChildren(&glyph, "guideline", guidelines);
  xml::writeChildren(&glyph, "anchor", anchors);
  xml::writeChild(&glyph, "outline", outline);
  xml::writeChild(&glyph, "lib", lib);
  boost::property_tree::ptree tree;
  tree.add_child("glyph", glyph);
  boost::property_tree::xml_writer_settings<std::string> settings(' ', 2);
  boost::property_tree::xml_parser::write_xml(stream, tree, settings);
  return true;
}

}  // namespace ufo
}  // namespace token
