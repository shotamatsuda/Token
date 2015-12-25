//
//  token/ufo/gasp_range_record.h
//
//  The MIT License
//
//  Copyright (C) 2015 Shota Matsuda
//
//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
//  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.
//

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
  static GASPRangeRecord read(const PropertyList& plist);
  PropertyList plist() const;

 public:
  unsigned int range_max_ppem;
  std::vector<unsigned int> range_gasp_behavior;
};

// Comparison
bool operator==(const GASPRangeRecord& lhs, const GASPRangeRecord& rhs);
bool operator!=(const GASPRangeRecord& lhs, const GASPRangeRecord& rhs);

#pragma mark -

inline GASPRangeRecord::GASPRangeRecord()
    : range_max_ppem() {}

#pragma mark Comparison

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
