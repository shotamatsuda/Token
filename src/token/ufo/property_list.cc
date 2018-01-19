// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#include "token/ufo/property_list.h"

extern "C" {

#include <plist/plist.h>

}  // extern "C"

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <iterator>
#include <ostream>
#include <string>

namespace token {
namespace ufo {

PropertyList::PropertyList() : plist_(plist_new_dict()), owner_(true) {}

PropertyList::PropertyList(std::istream& stream) : plist_(), owner_() {
  assert(stream.good());
  const std::istreambuf_iterator<char> first(stream);
  const std::string contents(first, std::istreambuf_iterator<char>());
  plist_t node{};
  plist_from_xml(contents.c_str(), contents.size(), &node);
  assert(node);
  assert(plist_get_node_type(node) == PLIST_DICT);
  plist_ = node;
  owner_ = true;
}

PropertyList::PropertyList(void *plist, bool owner)
    : plist_(plist),
      owner_(owner) {
  assert(plist_);
  assert(plist_get_node_type(plist_) == PLIST_DICT);
}

PropertyList::~PropertyList() {
  if (owner_ && plist_) {
    plist_free(plist_);
    plist_ = nullptr;
    owner_ = false;
  }
}

// MARK: Modifier

void PropertyList::save(std::ostream& stream) const {
  char *xml{};
  std::uint32_t length{};
  plist_to_xml(plist_, &xml, &length);
  stream << std::string(xml, length);
  std::free(xml);
}

}  // namespace ufo
}  // namespace token
