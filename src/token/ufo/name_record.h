// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

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

  // Property list
  explicit NameRecord(const PropertyList& plist);
  PropertyList plist() const;

 public:
  unsigned int name_id;
  unsigned int platform_id;
  unsigned int encoding_id;
  unsigned int language_id;
  std::string string;
};

// Comparison
bool operator==(const NameRecord& lhs, const NameRecord& rhs);
bool operator!=(const NameRecord& lhs, const NameRecord& rhs);

// MARK: -

inline NameRecord::NameRecord()
    : name_id(),
      platform_id(),
      encoding_id(),
      language_id() {}

// MARK: Comparison

inline bool operator==(const NameRecord& lhs, const NameRecord& rhs) {
  return (lhs.name_id == rhs.name_id &&
          lhs.platform_id == rhs.platform_id &&
          lhs.encoding_id == rhs.encoding_id &&
          lhs.language_id == rhs.language_id &&
          lhs.string == rhs.string);
}

inline bool operator!=(const NameRecord& lhs, const NameRecord& rhs) {
  return !(lhs == rhs);
}

}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_NAME_RECORD_H_
