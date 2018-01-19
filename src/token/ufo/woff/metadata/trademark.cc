// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#include "token/ufo/woff/metadata/trademark.h"

#include "token/ufo/plist.h"
#include "token/ufo/property_list.h"

namespace token {
namespace ufo {
namespace woff {
namespace metadata {

// MARK: Property list

Trademark::Trademark(const PropertyList& plist) {
  plist::readVector(plist, "text", &text);
}

PropertyList Trademark::plist() const {
  PropertyList plist;
  plist::writeVector(plist, "text", text);
  return plist;
}

}  // namespace metadata
}  // namespace woff
}  // namespace ufo
}  // namespace token
