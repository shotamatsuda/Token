// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#include "token/ufo/woff/metadata/extension_item.h"

#include "token/ufo/plist.h"
#include "token/ufo/property_list.h"

namespace token {
namespace ufo {
namespace woff {
namespace metadata {

// MARK: Property list

ExtensionItem::ExtensionItem(const PropertyList& plist) {
  plist::readString(plist, "identifier", &identifier);
  plist::readVector(plist, "names", &names);
  plist::readVector(plist, "values", &values);
}

PropertyList ExtensionItem::plist() const {
  PropertyList plist;
  plist::writeString(plist, "identifier", identifier);
  plist::writeVector(plist, "names", names);
  plist::writeVector(plist, "values", values);
  return plist;
}

}  // namespace metadata
}  // namespace woff
}  // namespace ufo
}  // namespace token
