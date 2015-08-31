//
//  token/ufo/name_record.h
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
#ifndef TOKEN_UFO_NAME_RECORD_H_
#define TOKEN_UFO_NAME_RECORD_H_

#include <string>

#include "token/ufo/property_list.h"

namespace token {
namespace ufo {

class NameRecord final {
 public:
  NameRecord();

  // Copy semantics
  NameRecord(const NameRecord&) = default;
  NameRecord& operator=(const NameRecord&) = default;

  // Comparison
  bool operator==(const NameRecord& other) const;
  bool operator!=(const NameRecord& other) const;

  // Property list
  static NameRecord read(const PropertyList& plist);
  PropertyList plist() const;

 public:
  unsigned int name_id;
  unsigned int platform_id;
  unsigned int encoding_id;
  unsigned int language_id;
  std::string string;
};

#pragma mark -

inline NameRecord::NameRecord()
    : name_id(),
      platform_id(),
      encoding_id(),
      language_id() {}

#pragma mark Comparison

inline bool NameRecord::operator==(const NameRecord& other) const {
  return (name_id == other.name_id &&
          platform_id == other.platform_id &&
          encoding_id == other.encoding_id &&
          language_id == other.language_id &&
          string == other.string);
}

inline bool NameRecord::operator!=(const NameRecord& other) const {
  return operator==(other);
}

}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_NAME_RECORD_H_
