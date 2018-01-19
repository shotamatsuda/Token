// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#pragma once
#ifndef TOKEN_UFO_WOFF_METADATA_CREDITS_H_
#define TOKEN_UFO_WOFF_METADATA_CREDITS_H_

#include <vector>

#include "token/ufo/property_list.h"
#include "token/ufo/woff/metadata/credit.h"

namespace token {
namespace ufo {
namespace woff {
namespace metadata {

class Credits final {
 public:
  Credits() = default;

  // Copy semantics
  Credits(const Credits&) = default;
  Credits& operator=(const Credits&) = default;

  // Property list
  explicit Credits(const PropertyList& plist);
  PropertyList plist() const;

 public:
  std::vector<Credit> credits;
};

// Comparison
bool operator==(const Credits& lhs, const Credits& rhs);
bool operator!=(const Credits& lhs, const Credits& rhs);

// MARK: -

// MARK: Comparison

inline bool operator==(const Credits& lhs, const Credits& rhs) {
  return lhs.credits == rhs.credits;
}

inline bool operator!=(const Credits& lhs, const Credits& rhs) {
  return !(lhs == rhs);
}

}  // namespace metadata
}  // namespace woff
}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_WOFF_METADATA_CREDITS_H_
