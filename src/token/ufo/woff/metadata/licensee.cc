// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#include "token/ufo/woff/metadata/licensee.h"

#include "token/ufo/plist.h"
#include "token/ufo/property_list.h"

namespace token {
namespace ufo {
namespace woff {
namespace metadata {

// MARK: Property list

Licensee::Licensee(const PropertyList& plist) {
  plist::readString(plist, "name", &name);
  plist::readString(plist, "dir", &dir);
  plist::readString(plist, "class", &klass);
}

PropertyList Licensee::plist() const {
  PropertyList plist;
  plist::writeString(plist, "name", name);
  plist::writeString(plist, "dir", dir);
  plist::writeString(plist, "class", klass);
  return plist;
}

}  // namespace metadata
}  // namespace woff
}  // namespace ufo
}  // namespace token
