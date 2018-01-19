// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#include "token/ufo/woff/metadata/description.h"

#include "token/ufo/plist.h"
#include "token/ufo/property_list.h"

namespace token {
namespace ufo {
namespace woff {
namespace metadata {

// MARK: Property list

Description::Description(const PropertyList& plist) {
  plist::readString(plist, "url", &url);
  plist::readVector(plist, "text", &text);
}

PropertyList Description::plist() const {
  PropertyList plist;
  plist::writeString(plist, "url", url);
  plist::writeVector(plist, "text", text);
  return plist;
}

}  // namespace metadata
}  // namespace woff
}  // namespace ufo
}  // namespace token
