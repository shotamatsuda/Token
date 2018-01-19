// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#include "token/ufo/gasp_range_record.h"

#include "token/ufo/plist.h"
#include "token/ufo/property_list.h"

namespace token {
namespace ufo {

// MARK: Property list

GASPRangeRecord::GASPRangeRecord(const PropertyList& plist)
    : GASPRangeRecord() {
  plist::readNumber(plist, "rangeMaxPPEM", &range_max_ppem);
  plist::readVector(plist, "rangeGaspBehavior", &range_gasp_behavior);
}

PropertyList GASPRangeRecord::plist() const {
  PropertyList plist;
  plist::writeNumber(plist, "rangeMaxPPEM", range_max_ppem);
  plist::writeVector(plist, "rangeGaspBehavior", range_gasp_behavior);
  return plist;
}

}  // namespace ufo
}  // namespace token
