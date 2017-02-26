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

#include "token/ufo/plist.h"
#include "token/ufo/property_list.h"

namespace token {
namespace ufo {

// MARK: Property list

NameRecord::NameRecord(const PropertyList& plist) : NameRecord() {
  plist::readNumber(plist, "nameID", &name_id);
  plist::readNumber(plist, "platformID", &platform_id);
  plist::readNumber(plist, "encodingID", &encoding_id);
  plist::readNumber(plist, "languageID", &language_id);
  plist::readString(plist, "string", &string);
}

PropertyList NameRecord::plist() const {
  PropertyList plist;
  plist::writeNumber(plist, "nameID", name_id);
  plist::writeNumber(plist, "platformID", platform_id);
  plist::writeNumber(plist, "encodingID", encoding_id);
  plist::writeNumber(plist, "languageID", language_id);
  plist::writeString(plist, "string", string);
  return plist;
}

}  // namespace ufo
}  // namespace token
