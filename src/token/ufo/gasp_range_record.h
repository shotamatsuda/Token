// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#pragma once
#ifndef TOKEN_UFO_GASP_RANGE_RECORD_H_
#define TOKEN_UFO_GASP_RANGE_RECORD_H_

#include <vector>

#include "token/ufo/property_list.h"

namespace token {
namespace ufo {

class GASPRangeRecord final {
 public:
  GASPRangeRecord();

  // Copy semantics
  GASPRangeRecord(const GASPRangeRecord&) = default;
  GASPRangeRecord& operator=(const GASPRangeRecord&) = default;

  // Property list
  explicit GASPRangeRecord(const PropertyList& plist);
  PropertyList plist() const;

 public:
  unsigned int range_max_ppem;
  std::vector<unsigned int> range_gasp_behavior;
};

// Comparison
bool operator==(const GASPRangeRecord& lhs, const GASPRangeRecord& rhs);
bool operator!=(const GASPRangeRecord& lhs, const GASPRangeRecord& rhs);

// MARK: -

inline GASPRangeRecord::GASPRangeRecord() : range_max_ppem() {}

// MARK: Comparison

inline bool operator==(const GASPRangeRecord& lhs, const GASPRangeRecord& rhs) {
  return (lhs.range_max_ppem == rhs.range_max_ppem &&
          lhs.range_gasp_behavior == rhs.range_gasp_behavior);
}

inline bool operator!=(const GASPRangeRecord& lhs, const GASPRangeRecord& rhs) {
  return !(lhs == rhs);
}

}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_GASP_RANGE_RECORD_H_
