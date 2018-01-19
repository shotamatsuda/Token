// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

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
  const auto result = open(stream);
  stream.close();
  path_ = glyphs;
  return result;
}

bool Glyphs::open(std::istream& stream) {
  if (!stream.good()) {
    return false;
  }
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
