// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#pragma once
#ifndef TOKEN_UFO_WOFF_METADATA_LICENSEE_H_
#define TOKEN_UFO_WOFF_METADATA_LICENSEE_H_

#include <string>

#include "token/ufo/property_list.h"

namespace token {
namespace ufo {
namespace woff {
namespace metadata {

class Licensee final {
 public:
  Licensee() = default;

  // Copy semantics
  Licensee(const Licensee&) = default;
  Licensee& operator=(const Licensee&) = default;

  // Property list
  explicit Licensee(const PropertyList& plist);
  PropertyList plist() const;

 public:
  std::string name;
  std::string dir;
  std::string klass;
};

// Comparison
bool operator==(const Licensee& lhs, const Licensee& rhs);
bool operator!=(const Licensee& lhs, const Licensee& rhs);

// MARK: -

// MARK: Comparison

inline bool operator==(const Licensee& lhs, const Licensee& rhs) {
  return lhs.name == rhs.name && lhs.dir == rhs.dir && lhs.klass == rhs.klass;
}

inline bool operator!=(const Licensee& lhs, const Licensee& rhs) {
  return !(lhs == rhs);
}

}  // namespace metadata
}  // namespace woff
}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_WOFF_METADATA_LICENSEE_H_
