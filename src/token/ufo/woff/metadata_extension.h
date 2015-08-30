//
//  token/ufo/woff/metadata_extension.h
//
//  The MIT License
//
//  Copyright (C) 2015 Shota Matsuda
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
#ifndef TOKEN_UFO_WOFF_METADATA_EXTENSION_H_
#define TOKEN_UFO_WOFF_METADATA_EXTENSION_H_

#include <string>
#include <vector>

#include "token/ufo/woff/metadata_extension_item.h"
#include "token/ufo/woff/metadata_extension_name.h"

namespace token {
namespace ufo {
namespace woff {

class MetadataExtension final {
 public:
  MetadataExtension();

  // Copy semantics
  MetadataExtension(const MetadataExtension&) = default;
  MetadataExtension& operator=(const MetadataExtension&) = default;

  // Comparison
  bool operator==(const MetadataExtension& other) const;
  bool operator!=(const MetadataExtension& other) const;

 public:
  std::string id;
  std::vector<MetadataExtensionName> names;
  std::vector<MetadataExtensionItem> items;
};

#pragma mark -

}  // namespace woff
}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_WOFF_METADATA_EXTENSION_H_
