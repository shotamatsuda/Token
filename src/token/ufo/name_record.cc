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

#include "token/ufo/name_record.h"

#include <utility>

#include "token/ufo/plist.h"
#include "token/ufo/property_list.h"

namespace token {
namespace ufo {

// MARK: Property list

NameRecord::NameRecord(const PropertyList& plist) : NameRecord() {
  plist::read_number(plist, "nameID", &name_id);
  plist::read_number(plist, "platformID", &platform_id);
  plist::read_number(plist, "encodingID", &encoding_id);
  plist::read_number(plist, "languageID", &language_id);
  plist::read_string(plist, "string", &string);
}

PropertyList NameRecord::plist() const {
  PropertyList plist;
  plist::write_number(plist, "nameID", name_id);
  plist::write_number(plist, "platformID", platform_id);
  plist::write_number(plist, "encodingID", encoding_id);
  plist::write_number(plist, "languageID", language_id);
  plist::write_string(plist, "string", string);
  return plist;
}

}  // namespace ufo
}  // namespace token
