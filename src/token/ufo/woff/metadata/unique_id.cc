// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#include "token/ufo/woff/metadata/unique_id.h"

#include "token/ufo/plist.h"
#include "token/ufo/property_list.h"

namespace token {
namespace ufo {
namespace woff {
namespace metadata {

// MARK: Property list

UniqueID::UniqueID(const PropertyList& plist) {
  plist::readString(plist, "identifier", &identifier);
}

PropertyList UniqueID::plist() const {
  PropertyList plist;
  plist::writeString(plist, "identifier", identifier);
  return plist;
}

}  // namespace metadata
}  // namespace woff
}  // namespace ufo
}  // namespace token
