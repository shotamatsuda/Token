// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#pragma once
#ifndef TOKEN_UFO_WOFF_METADATA_UNIQUE_ID_H_
#define TOKEN_UFO_WOFF_METADATA_UNIQUE_ID_H_

#include <string>

#include "token/ufo/property_list.h"

namespace token {
namespace ufo {
namespace woff {
namespace metadata {

class UniqueID final {
 public:
  UniqueID() = default;

  // Copy semantics
  UniqueID(const UniqueID&) = default;
  UniqueID& operator=(const UniqueID&) = default;

  // Property list
  explicit UniqueID(const PropertyList& plist);
  PropertyList plist() const;

 public:
  std::string identifier;
};

// Comparison
bool operator==(const UniqueID& lhs, const UniqueID& rhs);
bool operator!=(const UniqueID& lhs, const UniqueID& rhs);

// MARK: -

// MARK: Comparison

inline bool operator==(const UniqueID& lhs, const UniqueID& rhs) {
  return lhs.identifier == rhs.identifier;
}

inline bool operator!=(const UniqueID& lhs, const UniqueID& rhs) {
  return !(lhs == rhs);
}

}  // namespace metadata
}  // namespace woff
}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_WOFF_METADATA_UNIQUE_ID_H_
