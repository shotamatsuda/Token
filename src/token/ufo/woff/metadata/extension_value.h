// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#pragma once
#ifndef TOKEN_UFO_WOFF_METADATA_EXTENSION_VALUE_H_
#define TOKEN_UFO_WOFF_METADATA_EXTENSION_VALUE_H_

#include <string>

#include "token/ufo/property_list.h"

namespace token {
namespace ufo {
namespace woff {
namespace metadata {

class ExtensionValue final {
 public:
  ExtensionValue() = default;

  // Copy semantics
  ExtensionValue(const ExtensionValue&) = default;
  ExtensionValue& operator=(const ExtensionValue&) = default;

  // Property list
  explicit ExtensionValue(const PropertyList& plist);
  PropertyList plist() const;

 public:
  std::string text;
  std::string language;
  std::string dir;
  std::string klass;
};

// Comparison
bool operator==(const ExtensionValue& lhs, const ExtensionValue& rhs);
bool operator!=(const ExtensionValue& lhs, const ExtensionValue& rhs);

// MARK: -

// MARK: Comparison

inline bool operator==(const ExtensionValue& lhs, const ExtensionValue& rhs) {
  return (lhs.text == rhs.text &&
          lhs.language == rhs.language &&
          lhs.dir == rhs.dir &&
          lhs.klass == rhs.klass);
}

inline bool operator!=(const ExtensionValue& lhs, const ExtensionValue& rhs) {
  return !(lhs == rhs);
}

}  // namespace metadata
}  // namespace woff
}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_WOFF_METADATA_EXTENSION_VALUE_H_
