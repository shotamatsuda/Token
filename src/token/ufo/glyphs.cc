//
//  token/ufo/glyphs.cc
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

#include "token/ufo/glyphs.h"

extern "C" {

#include <plist/plist.h>

}  // extern "C"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <iterator>
#include <fstream>
#include <string>
#include <utility>

#include <boost/filesystem/path.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "token/ufo/glyph.h"

namespace token {
namespace ufo {

// MARK: Opening

bool Glyphs::open(const std::string& path) {
  const boost::filesystem::path node(path);
  std::string glyphs = node.parent_path().string();
  std::string contents = path;
  if (node.leaf().extension() == ".ufo") {
    glyphs = (node / "glyphs").string();
    contents = (node / "glyphs" / "contents.plist").string();
  }
  std::ifstream stream(contents);
  const auto result = open(&stream);
  stream.close();
  path_ = glyphs;
  return result;
}

bool Glyphs::open(std::istream *stream) {
  assert(stream);
  PropertyList plist(stream);
  plist_dict_iter itr{};
  plist_dict_new_iter(plist, &itr);
  const auto size = plist_dict_get_size(plist);
  for (std::uint32_t i{}; i < size; ++i) {
    char *key{};
    plist_t item{};
    plist_dict_next_item(plist, itr, &key, &item);
    assert(key);
    assert(plist_get_node_type(item) == PLIST_STRING);
    char *value{};
    plist_get_string_val(item, &value);
    contents_.emplace_back(key, value);
    std::free(key);
    std::free(value);
  }
  std::free(itr);
  return true;
}

// MARK: Glyphs

std::string Glyphs::filename(const std::string& name) const {
  const auto value = std::find_if(
      std::begin(contents_), std::end(contents_),
      [&name](const auto& pair) {
        return pair.first == name;
      });
  if (value == std::end(contents_)) {
    return std::string();
  }
  return value->second;
}

const Glyph * Glyphs::find(const std::string& name) const {
  const auto itr = std::find_if(
      std::begin(glyphs_), std::end(glyphs_),
      [&name](const auto& pair) {
        return pair.first == name;
      });
  if (itr != std::end(glyphs_)) {
    return &itr->second;
  }
  const auto filename = this->filename(name);
  if (filename.empty()) {
    return nullptr;
  }
  const auto path = boost::filesystem::path(path_) / filename;
  Glyph glyph;
  if (!glyph.open(path.string())) {
    return nullptr;
  }
  glyphs_.emplace_back(name, std::move(glyph));
  return &glyphs_.back().second;
}

Glyph * Glyphs::find(const std::string& name) {
  return const_cast<Glyph *>(const_cast<const Glyphs *>(this)->find(name));
}

void Glyphs::set(const std::string& name, const Glyph& glyph) {
  const auto content_itr = std::find_if(
      std::begin(contents_), std::end(contents_),
      [&name](const auto& pair) {
        return pair.first == name;
      });
  if (content_itr == std::end(contents_)) {
    return;  // Setting a new glyph is not supported
  }
  const auto glyph_itr = std::find_if(
      std::begin(glyphs_), std::end(glyphs_),
      [&name](const auto& pair) {
        return pair.first == name;
      });
  if (glyph_itr != std::end(glyphs_)) {
    glyph_itr->second = glyph;
  } else {
    glyphs_.emplace_back(name, glyph);
  }
}

}  // namespace ufo
}  // namespace token
