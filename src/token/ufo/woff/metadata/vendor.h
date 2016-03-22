//
//  token/ufo/woff/metadata/vendor.h
//
//  The MIT License
//
//  Copyright (C) 2015-2016 Shota Matsuda
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
  static Vendor read(const PropertyList& plist);
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

#pragma mark -

#pragma mark Comparison

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
