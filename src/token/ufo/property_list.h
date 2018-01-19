// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

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
  explicit PropertyList(std::istream& stream);
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
  void save(std::ostream& stream) const;

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
