//
//  token/ufo/property_list.cc
//
//  MIT License
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

PropertyList::PropertyList(std::istream *stream) : plist_(), owner_() {
  assert(stream);
  assert(stream->good());
  const std::istreambuf_iterator<char> first(*stream);
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

void PropertyList::save(std::ostream *stream) const {
  char *xml{};
  std::uint32_t length{};
  plist_to_xml(plist_, &xml, &length);
  *stream << std::string(xml, length);
  std::free(xml);
}

}  // namespace ufo
}  // namespace token
