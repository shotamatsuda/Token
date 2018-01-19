// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#pragma once
#ifndef TOKEN_UFO_WOFF_METADATA_LICENSE_H_
#define TOKEN_UFO_WOFF_METADATA_LICENSE_H_

#include <string>
#include <vector>

#include "token/ufo/property_list.h"
#include "token/ufo/woff/metadata/text.h"

namespace token {
namespace ufo {
namespace woff {
namespace metadata {

class License final {
 public:
  License() = default;

  // Copy semantics
  License(const License&) = default;
  License& operator=(const License&) = default;

  // Property list
  explicit License(const PropertyList& plist);
  PropertyList plist() const;

 public:
  std::string url;
  std::string identifier;
  std::vector<Text> text;
};

// Comparison
bool operator==(const License& lhs, const License& rhs);
bool operator!=(const License& lhs, const License& rhs);

// MARK: -

// MARK: Comparison

inline bool operator==(const License& lhs, const License& rhs) {
  return (lhs.url == rhs.url &&
          lhs.identifier == rhs.identifier &&
          lhs.text == rhs.text);
}

inline bool operator!=(const License& lhs, const License& rhs) {
  return !(lhs == rhs);
}

}  // namespace metadata
}  // namespace woff
}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_WOFF_METADATA_LICENSE_H_
