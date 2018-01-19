// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#pragma once
#ifndef TOKEN_UFO_WOFF_METADATA_EXTENSION_NAME_H_
#define TOKEN_UFO_WOFF_METADATA_EXTENSION_NAME_H_

#include <string>

#include "token/ufo/property_list.h"

namespace token {
namespace ufo {
namespace woff {
namespace metadata {

class ExtensionName final {
 public:
  ExtensionName() = default;

  // Copy semantics
  ExtensionName(const ExtensionName&) = default;
  ExtensionName& operator=(const ExtensionName&) = default;

  // Property list
  explicit ExtensionName(const PropertyList& plist);
  PropertyList plist() const;

 public:
  std::string text;
  std::string language;
  std::string dir;
  std::string klass;
};

// Comparison
bool operator==(const ExtensionName& lhs, const ExtensionName& rhs);
bool operator!=(const ExtensionName& lhs, const ExtensionName& rhs);

// MARK: -

// MARK: Comparison

inline bool operator==(const ExtensionName& lhs, const ExtensionName& rhs) {
  return (lhs.text == rhs.text &&
          lhs.language == rhs.language &&
          lhs.dir == rhs.dir &&
          lhs.klass == rhs.klass);
}

inline bool operator!=(const ExtensionName& lhs, const ExtensionName& rhs) {
  return !(lhs == rhs);
}

}  // namespace metadata
}  // namespace woff
}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_WOFF_METADATA_EXTENSION_NAME_H_
