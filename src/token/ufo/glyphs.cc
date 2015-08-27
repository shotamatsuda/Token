//
//  token/ufo/glyphs.cc
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

#include "token/ufo/glyphs.h"

extern "C" {

#include <plist/plist.h>

}  // extern "C"

#include <cassert>
#include <iterator>
#include <fstream>
#include <string>

#include <boost/filesystem/path.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "token/ufo/glif/glyph.h"

namespace token {
namespace ufo {

plist_t Glyphs::openPropertyList(const std::string& file) const {
  std::ifstream stream((boost::filesystem::path(path_) / file).string());
  if (!stream.good()) {
    return nullptr;
  }
  const std::istreambuf_iterator<char> first(stream);
  const std::string contents(first, std::istreambuf_iterator<char>());
  plist_t plist{};
  plist_from_xml(contents.c_str(), contents.size(), &plist);
  if (!plist) {
    return nullptr;
  }
  assert(plist_get_node_type(plist) == PLIST_DICT);
  return plist;
}

#pragma mark Glyphs

const Glyph& Glyphs::get(const std::string& name) const {
  auto glyph = find(name);
  assert(glyph);
  return *glyph;
}

Glyph& Glyphs::get(const std::string& name) {
  return const_cast<Glyph&>(const_cast<const Glyphs *>(this)->get(name));
}

const Glyph * Glyphs::find(const std::string& name) const {
  const auto itr = glyphs_.find(name);
  if (itr != std::end(glyphs_)) {
    return &itr->second;
  }
  auto stream = openGLIF(name);
  if (!stream.good()) {
    return nullptr;
  }
  auto glyph = readGlyph(&stream);
  const auto result = glyphs_.emplace(std::make_pair(name, std::move(glyph)));
  assert(result.second);
  return &result.first->second;
}

Glyph * Glyphs::find(const std::string& name) {
  return const_cast<Glyph *>(const_cast<const Glyphs *>(this)->find(name));
}

std::ifstream Glyphs::openGLIF(const std::string& name) const {
  const auto node = plist_dict_get_item(contents_, name.c_str());
  if (!node) {
    return std::ifstream(nullptr);
  }
  assert(plist_get_node_type(node) == PLIST_STRING);
  char *file_name{};
  plist_get_string_val(node, &file_name);
  if (!file_name) {
    return std::ifstream(nullptr);
  }
  return std::ifstream((boost::filesystem::path(path_) / file_name).string());
}

Glyph Glyphs::readGlyph(std::ifstream *stream) const {
  assert(stream);
  assert(stream->good());
  boost::property_tree::ptree tree;
  boost::property_tree::xml_parser::read_xml(*stream, tree);
  return Glyph::read(tree);
}

}  // namespace ufo
}  // namespace token
