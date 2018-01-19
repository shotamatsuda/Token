// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

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
