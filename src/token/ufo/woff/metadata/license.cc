// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#include "token/ufo/woff/metadata/license.h"

#include "token/ufo/plist.h"
#include "token/ufo/property_list.h"

namespace token {
namespace ufo {
namespace woff {
namespace metadata {

// MARK: Property list

License::License(const PropertyList& plist) {
  plist::readString(plist, "url", &url);
  plist::readString(plist, "identifier", &identifier);
  plist::readVector(plist, "text", &text);
}

PropertyList License::plist() const {
  PropertyList plist;
  plist::writeString(plist, "url", url);
  plist::writeString(plist, "identifier", identifier);
  plist::writeVector(plist, "text", text);
  return plist;
}

}  // namespace metadata
}  // namespace woff
}  // namespace ufo
}  // namespace token
