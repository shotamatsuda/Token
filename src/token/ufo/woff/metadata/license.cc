//
//  token/ufo/woff/metadata/license.cc
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

#include "token/ufo/woff/metadata/license.h"

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

License License::read(const PropertyList& plist) {
  assert(plist_get_node_type(plist) == PLIST_DICT);
  License result;
  plist::read_string(plist, "url", &result.url);
  plist::read_string(plist, "identifier", &result.identifier);
  plist::read_vector(plist, "text", &result.text);
  return std::move(result);
}

PropertyList License::plist() const {
  plist_t plist = plist_new_dict();
  plist::write_string(plist, "url", url);
  plist::write_string(plist, "identifier", identifier);
  plist::write_vector(plist, "text", text);
  return PropertyList(plist);
}

}  // namespace metadata
}  // namespace woff
}  // namespace ufo
}  // namespace token
