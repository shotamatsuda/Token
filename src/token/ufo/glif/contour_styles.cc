// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#include "token/ufo/glif/contour_styles.h"

extern "C" {

#include <plist/plist.h>

}  // extern "C"

#include <cassert>
#include <cstdint>
#include <cstdlib>

#include "token/ufo/glif/contour_style.h"
#include "token/ufo/property_list.h"

namespace token {
namespace ufo {
namespace glif {

// MARK: Property list

ContourStyles::ContourStyles(const PropertyList& plist) {
  plist_dict_iter itr{};
  plist_dict_new_iter(plist, &itr);
  const auto size = plist_dict_get_size(plist);
  for (std::uint32_t i{}; i < size; ++i) {
    char *key{};
    plist_t item{};
    plist_dict_next_item(plist, itr, &key, &item);
    assert(key);
    assert(plist_get_node_type(item) == PLIST_DICT);
    styles_.emplace(key, ContourStyle(PropertyList(item, false)));
    std::free(key);
  }
  std::free(itr);
}

PropertyList ContourStyles::plist() const {
  PropertyList plist;
  return plist;
}

// MARK: Property tree

boost::property_tree::ptree ContourStyles::ptree() const {
  boost::property_tree::ptree tree;
  for (const auto& pair : styles_) {
    tree.add("key", pair.first);
    tree.add_child("dict", pair.second.ptree());
  }
  return tree;
}

}  // namespace glif
}  // namespace ufo
}  // namespace token
