// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#pragma once
#ifndef TOKEN_UFO_WOFF_METADATA_VENDOR_H_
#define TOKEN_UFO_WOFF_METADATA_VENDOR_H_

#include <string>

#include "token/ufo/property_list.h"

namespace token {
namespace ufo {
namespace woff {
namespace metadata {

class Vendor final {
 public:
  Vendor() = default;

  // Copy semantics
  Vendor(const Vendor&) = default;
  Vendor& operator=(const Vendor&) = default;

  // Property list
  explicit Vendor(const PropertyList& plist);
  PropertyList plist() const;

 public:
  std::string name;
  std::string url;
  std::string dir;
  std::string klass;
};

// Comparison
bool operator==(const Vendor& lhs, const Vendor& rhs);
bool operator!=(const Vendor& lhs, const Vendor& rhs);

// MARK: -

// MARK: Comparison

inline bool operator==(const Vendor& lhs, const Vendor& rhs) {
  return (lhs.name == rhs.name &&
          lhs.url == rhs.url &&
          lhs.dir == rhs.dir &&
          lhs.klass == rhs.klass);
}

inline bool operator!=(const Vendor& lhs, const Vendor& rhs) {
  return !(lhs == rhs);
}

}  // namespace metadata
}  // namespace woff
}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_WOFF_METADATA_VENDOR_H_
