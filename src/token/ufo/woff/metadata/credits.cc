// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#include "token/ufo/woff/metadata/credits.h"

#include "token/ufo/plist.h"
#include "token/ufo/property_list.h"

namespace token {
namespace ufo {
namespace woff {
namespace metadata {

// MARK: Property list

Credits::Credits(const PropertyList& plist) {
  plist::readVector(plist, "credits", &credits);
}

PropertyList Credits::plist() const {
  PropertyList plist;
  plist::writeVector(plist, "credits", credits);
  return plist;
}

}  // namespace metadata
}  // namespace woff
}  // namespace ufo
}  // namespace token
