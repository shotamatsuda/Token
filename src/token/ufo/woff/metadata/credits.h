//
//  token/ufo/woff/metadata/credits.h
//
//  The MIT License
//
//  Copyright (C) 2015-2017 Shota Matsuda
//
//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
//  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.
//

#pragma once
#ifndef TOKEN_UFO_WOFF_METADATA_CREDITS_H_
#define TOKEN_UFO_WOFF_METADATA_CREDITS_H_

#include <vector>

#include "token/ufo/property_list.h"
#include "token/ufo/woff/metadata/credit.h"

namespace token {
namespace ufo {
namespace woff {
namespace metadata {

class Credits final {
 public:
  Credits() = default;

  // Copy semantics
  Credits(const Credits&) = default;
  Credits& operator=(const Credits&) = default;

  // Concept: Property list representable
  explicit Credits(const PropertyList& plist);
  PropertyList plist() const;

 public:
  std::vector<Credit> credits;
};

// Comparison
bool operator==(const Credits& lhs, const Credits& rhs);
bool operator!=(const Credits& lhs, const Credits& rhs);

// MARK: -

// MARK: Comparison

inline bool operator==(const Credits& lhs, const Credits& rhs) {
  return lhs.credits == rhs.credits;
}

inline bool operator!=(const Credits& lhs, const Credits& rhs) {
  return !(lhs == rhs);
}

}  // namespace metadata
}  // namespace woff
}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_WOFF_METADATA_CREDITS_H_
