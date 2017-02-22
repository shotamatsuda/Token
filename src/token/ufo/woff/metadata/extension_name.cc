//
//  token/ufo/woff/metadata/extension_name.cc
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

#include "token/ufo/woff/metadata/extension_name.h"

#include <utility>

#include "token/ufo/plist.h"
#include "token/ufo/property_list.h"

namespace token {
namespace ufo {
namespace woff {
namespace metadata {

// MARK: Property list

ExtensionName::ExtensionName(const PropertyList& plist) {
  plist::read_string(plist, "text", &text);
  plist::read_string(plist, "language", &language);
  plist::read_string(plist, "dir", &dir);
  plist::read_string(plist, "class", &klass);
}

PropertyList ExtensionName::plist() const {
  PropertyList plist;
  plist::write_string(plist, "text", text);
  plist::write_string(plist, "language", language);
  plist::write_string(plist, "dir", dir);
  plist::write_string(plist, "class", klass);
  return std::move(plist);
}

}  // namespace metadata
}  // namespace woff
}  // namespace ufo
}  // namespace token
