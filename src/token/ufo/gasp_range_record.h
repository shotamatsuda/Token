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

namespace token {
namespace ufo {

class GASPRangeRecord final {
 public:
  GASPRangeRecord();

  // Copy semantics
  GASPRangeRecord(const GASPRangeRecord&) = default;
  GASPRangeRecord& operator=(const GASPRangeRecord&) = default;

  // Comparison
  bool operator==(const GASPRangeRecord& other) const;
  bool operator!=(const GASPRangeRecord& other) const;

 public:
  unsigned int range_max_ppem;
  std::vector<unsigned int> range_gasp_behavior;
};

#pragma mark -

inline GASPRangeRecord::GASPRangeRecord()
    : range_max_ppem() {}

#pragma mark Comparison

inline bool GASPRangeRecord::operator==(const GASPRangeRecord& other) const {
  return (range_max_ppem == other.range_max_ppem &&
          range_gasp_behavior == other.range_gasp_behavior);
}

inline bool GASPRangeRecord::operator!=(const GASPRangeRecord& other) const {
  return operator==(other);
}

}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_GASP_RANGE_RECORD_H_
