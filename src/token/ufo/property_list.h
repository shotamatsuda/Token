//
//  token/ufo/property_list.h
//
//  MIT License
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
#ifndef TOKEN_UFO_PROPERTY_LIST_H_
#define TOKEN_UFO_PROPERTY_LIST_H_

#include <istream>
#include <ostream>
#include <utility>

namespace token {
namespace ufo {

class PropertyList final {
 public:
  PropertyList();
  explicit PropertyList(std::istream *stream);
  explicit PropertyList(void *plist, bool owner = true);
  ~PropertyList();

  // Disallow copy semantics
  PropertyList(const PropertyList&) = delete;
  PropertyList& operator=(const PropertyList&) = delete;

  // Move semantics
  PropertyList(PropertyList&&);
  PropertyList& operator=(PropertyList&&);

  // Value access
  void * get() const { return plist_; }
  void * release();
  operator void *() const { return plist_; }

  // Modifier
  void save(std::ostream *stream) const;

 public:
  void *plist_;
  bool owner_;
};

// MARK: -

// MARK: Move semantics

inline PropertyList::PropertyList(PropertyList&& other)
    : plist_(other.plist_),
      owner_(other.owner_) {
  other.plist_ = nullptr;
  other.owner_ = false;
}

inline PropertyList& PropertyList::operator=(PropertyList&& other) {
  if (&other != this) {
    std::swap(plist_, other.plist_);
    std::swap(owner_, other.owner_);
  }
  return *this;
}

// MARK: Value access

inline void * PropertyList::release() {
  void *plist;
  std::swap(plist, plist_);
  plist_ = nullptr;
  owner_ = false;
  return plist;
}

}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_PROPERTY_LIST_H_
