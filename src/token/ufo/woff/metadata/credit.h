// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#pragma once
#ifndef TOKEN_UFO_WOFF_METADATA_CREDIT_H_
#define TOKEN_UFO_WOFF_METADATA_CREDIT_H_

#include <string>

#include "token/ufo/property_list.h"

namespace token {
namespace ufo {
namespace woff {
namespace metadata {

class Credit final {
 public:
  Credit() = default;

  // Copy semantics
  Credit(const Credit&) = default;
  Credit& operator=(const Credit&) = default;

  // Property list
  explicit Credit(const PropertyList& plist);
  PropertyList plist() const;

 public:
  std::string name;
  std::string url;
  std::string dir;
  std::string klass;
};

// Comparison
bool operator==(const Credit& lhs, const Credit& rhs);
bool operator!=(const Credit& lhs, const Credit& rhs);

// MARK: -

// MARK: Comparison

inline bool operator==(const Credit& lhs, const Credit& rhs) {
  return (lhs.name == rhs.name &&
          lhs.url == rhs.url &&
          lhs.dir == rhs.dir &&
          lhs.klass == rhs.klass);
}

inline bool operator!=(const Credit& lhs, const Credit& rhs) {
  return !(lhs == rhs);
}

}  // namespace metadata
}  // namespace woff
}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_WOFF_METADATA_CREDIT_H_
