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
