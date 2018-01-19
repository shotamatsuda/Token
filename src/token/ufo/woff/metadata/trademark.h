// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#pragma once
#ifndef TOKEN_UFO_WOFF_METADATA_TRADEMARK_H_
#define TOKEN_UFO_WOFF_METADATA_TRADEMARK_H_

#include <vector>

#include "token/ufo/property_list.h"
#include "token/ufo/woff/metadata/text.h"

namespace token {
namespace ufo {
namespace woff {
namespace metadata {

class Trademark final {
 public:
  Trademark() = default;

  // Copy semantics
  Trademark(const Trademark&) = default;
  Trademark& operator=(const Trademark&) = default;

  // Property list
  explicit Trademark(const PropertyList& plist);
  PropertyList plist() const;

 public:
  std::vector<Text> text;
};

// Comparison
bool operator==(const Trademark& lhs, const Trademark& rhs);
bool operator!=(const Trademark& lhs, const Trademark& rhs);

// MARK: -

// MARK: Comparison

inline bool operator==(const Trademark& lhs, const Trademark& rhs) {
  return lhs.text == rhs.text;
}

inline bool operator!=(const Trademark& lhs, const Trademark& rhs) {
  return !(lhs == rhs);
}

}  // namespace metadata
}  // namespace woff
}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_WOFF_METADATA_TRADEMARK_H_
