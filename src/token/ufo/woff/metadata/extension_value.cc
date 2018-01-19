// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#include "token/ufo/woff/metadata/extension_value.h"

#include "token/ufo/plist.h"
#include "token/ufo/property_list.h"

namespace token {
namespace ufo {
namespace woff {
namespace metadata {

// MARK: Property list

ExtensionValue::ExtensionValue(const PropertyList& plist) {
  plist::readString(plist, "text", &text);
  plist::readString(plist, "language", &language);
  plist::readString(plist, "dir", &dir);
  plist::readString(plist, "class", &klass);
}

PropertyList ExtensionValue::plist() const {
  PropertyList plist;
  plist::writeString(plist, "text", text);
  plist::writeString(plist, "language", language);
  plist::writeString(plist, "dir", dir);
  plist::writeString(plist, "class", klass);
  return plist;
}

}  // namespace metadata
}  // namespace woff
}  // namespace ufo
}  // namespace token
