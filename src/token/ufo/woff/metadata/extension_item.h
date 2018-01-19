// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#pragma once
#ifndef TOKEN_UFO_WOFF_METADATA_EXTENSION_ITEM_H_
#define TOKEN_UFO_WOFF_METADATA_EXTENSION_ITEM_H_

#include <string>
#include <vector>

#include "token/ufo/property_list.h"
#include "token/ufo/woff/metadata/extension_name.h"
#include "token/ufo/woff/metadata/extension_value.h"

namespace token {
namespace ufo {
namespace woff {
namespace metadata {

class ExtensionItem final {
 public:
  ExtensionItem() = default;

  // Copy semantics
  ExtensionItem(const ExtensionItem&) = default;
  ExtensionItem& operator=(const ExtensionItem&) = default;

  // Property list
  explicit ExtensionItem(const PropertyList& plist);
  PropertyList plist() const;

 public:
  std::string identifier;
  std::vector<ExtensionName> names;
  std::vector<ExtensionValue> values;
};

// Comparison
bool operator==(const ExtensionItem& lhs, const ExtensionItem& rhs);
bool operator!=(const ExtensionItem& lhs, const ExtensionItem& rhs);

// MARK: -

// MARK: Comparison

inline bool operator==(const ExtensionItem& lhs, const ExtensionItem& rhs) {
  return (lhs.identifier == rhs.identifier &&
          lhs.names == rhs.names &&
          lhs.values == rhs.values);
}

inline bool operator!=(const ExtensionItem& lhs, const ExtensionItem& rhs) {
  return !(lhs == rhs);
}

}  // namespace metadata
}  // namespace woff
}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_WOFF_METADATA_EXTENSION_ITEM_H_
