//
//  token/ufo/woff/metadata/extension_item.cc
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

#include "token/ufo/woff/metadata/extension_item.h"

extern "C" {

#include <plist/plist.h>

}  // extern "C"

#include <utility>

#include "token/ufo/plist.h"
#include "token/ufo/property_list.h"

namespace token {
namespace ufo {
namespace woff {
namespace metadata {

#pragma mark Property list

ExtensionItem ExtensionItem::read(const PropertyList& plist) {
  assert(plist_get_node_type(plist) == PLIST_DICT);
  ExtensionItem result;
  plist::read_string(plist, "identifier", &result.identifier);
  plist::read_vector(plist, "names", &result.names);
  plist::read_vector(plist, "values", &result.values);
  return std::move(result);
}

PropertyList ExtensionItem::plist() const {
  plist_t plist = plist_new_dict();
  plist::write_string(plist, "identifier", identifier);
  plist::write_vector(plist, "names", names);
  plist::write_vector(plist, "values", values);
  return PropertyList(plist);
}

}  // namespace metadata
}  // namespace woff
}  // namespace ufo
}  // namespace token
