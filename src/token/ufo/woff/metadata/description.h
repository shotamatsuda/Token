// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#pragma once
#ifndef TOKEN_UFO_WOFF_METADATA_DESCRIPTION_H_
#define TOKEN_UFO_WOFF_METADATA_DESCRIPTION_H_

#include <string>
#include <vector>

#include "token/ufo/property_list.h"
#include "token/ufo/woff/metadata/text.h"

namespace token {
namespace ufo {
namespace woff {
namespace metadata {

class Description final {
 public:
  Description() = default;

  // Copy semantics
  Description(const Description&) = default;
  Description& operator=(const Description&) = default;

  // Property list
  explicit Description(const PropertyList& plist);
  PropertyList plist() const;

 public:
  std::string url;
  std::vector<Text> text;
};

// Comparison
bool operator==(const Description& lhs, const Description& rhs);
bool operator!=(const Description& lhs, const Description& rhs);

// MARK: -

// MARK: Comparison

inline bool operator==(const Description& lhs, const Description& rhs) {
  return lhs.url == rhs.url && lhs.text == rhs.text;
}

inline bool operator!=(const Description& lhs, const Description& rhs) {
  return !(lhs == rhs);
}

}  // namespace metadata
}  // namespace woff
}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_WOFF_METADATA_DESCRIPTION_H_
