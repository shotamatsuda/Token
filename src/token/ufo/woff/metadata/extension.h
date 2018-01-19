// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#pragma once
#ifndef TOKEN_UFO_WOFF_METADATA_EXTENSION_H_
#define TOKEN_UFO_WOFF_METADATA_EXTENSION_H_

#include <string>
#include <vector>

#include "token/ufo/property_list.h"
#include "token/ufo/woff/metadata/extension_item.h"
#include "token/ufo/woff/metadata/extension_name.h"

namespace token {
namespace ufo {
namespace woff {
namespace metadata {

class Extension final {
 public:
  Extension() = default;

  // Copy semantics
  Extension(const Extension&) = default;
  Extension& operator=(const Extension&) = default;

  // Property list
  explicit Extension(const PropertyList& plist);
  PropertyList plist() const;

 public:
  std::string identifier;
  std::vector<ExtensionName> names;
  std::vector<ExtensionItem> items;
};

// Comparison
bool operator==(const Extension& lhs, const Extension& rhs);
bool operator!=(const Extension& lhs, const Extension& rhs);

// MARK: -

// MARK: Comparison

inline bool operator==(const Extension& lhs, const Extension& rhs) {
  return (lhs.identifier == rhs.identifier &&
          lhs.names == rhs.names &&
          lhs.items == rhs.items);
}

inline bool operator!=(const Extension& lhs, const Extension& rhs) {
  return !(lhs == rhs);
}

}  // namespace metadata
}  // namespace woff
}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_WOFF_METADATA_EXTENSION_H_
