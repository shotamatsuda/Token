// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#include "token/ufo/guideline.h"

#include "token/ufo/plist.h"
#include "token/ufo/property_list.h"

namespace token {
namespace ufo {

// MARK: Property list

Guideline::Guideline(const PropertyList& plist) {
  plist::readNumber(plist, "x", &x);
  plist::readNumber(plist, "y", &y);
  plist::readNumber(plist, "angle", &angle);
  plist::readString(plist, "name", &name);
  plist::readString(plist, "color", &color);
  plist::readString(plist, "identifier", &identifier);
}

PropertyList Guideline::plist() const {
  return PropertyList();
}

}  // namespace ufo
}  // namespace token
