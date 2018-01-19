// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#pragma once
#ifndef TOKEN_UFO_WOFF_METADATA_TEXT_H_
#define TOKEN_UFO_WOFF_METADATA_TEXT_H_

#include <string>

#include "token/ufo/property_list.h"

namespace token {
namespace ufo {
namespace woff {
namespace metadata {

class Text final {
 public:
  Text() = default;

  // Copy semantics
  Text(const Text&) = default;
  Text& operator=(const Text&) = default;

  // Property list
  explicit Text(const PropertyList& plist);
  PropertyList plist() const;

 public:
  std::string text;
  std::string language;
  std::string dir;
  std::string klass;
};

// Comparison
bool operator==(const Text& lhs, const Text& rhs);
bool operator!=(const Text& lhs, const Text& rhs);

// MARK: -

// MARK: Comparison

inline bool operator==(const Text& lhs, const Text& rhs) {
  return (lhs.text == rhs.text &&
          lhs.language == rhs.language &&
          lhs.dir == rhs.dir &&
          lhs.klass == rhs.klass);
}

inline bool operator!=(const Text& lhs, const Text& rhs) {
  return !(lhs == rhs);
}

}  // namespace metadata
}  // namespace woff
}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_WOFF_METADATA_TEXT_H_
