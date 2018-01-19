// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#pragma once
#ifndef TOKEN_UFO_WOFF_METADATA_COPYRIGHT_H_
#define TOKEN_UFO_WOFF_METADATA_COPYRIGHT_H_

#include <vector>

#include "token/ufo/property_list.h"
#include "token/ufo/woff/metadata/text.h"

namespace token {
namespace ufo {
namespace woff {
namespace metadata {

class Copyright final {
 public:
  Copyright() = default;

  // Copy semantics
  Copyright(const Copyright&) = default;
  Copyright& operator=(const Copyright&) = default;

  // Property list
  explicit Copyright(const PropertyList& plist);
  PropertyList plist() const;

 public:
  std::vector<Text> text;
};

// Comparison
bool operator==(const Copyright& lhs, const Copyright& rhs);
bool operator!=(const Copyright& lhs, const Copyright& rhs);

// MARK: -

// MARK: Comparison

inline bool operator==(const Copyright& lhs, const Copyright& rhs) {
  return lhs.text == rhs.text;
}

inline bool operator!=(const Copyright& lhs, const Copyright& rhs) {
  return !(lhs == rhs);
}

}  // namespace metadata
}  // namespace woff
}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_WOFF_METADATA_COPYRIGHT_H_
