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
#ifndef TOKEN_UFO_OPTIONAL_H_
#define TOKEN_UFO_OPTIONAL_H_

#include <cassert>
#include <utility>

namespace token {
namespace ufo {

template <class T>
class Optional final {
 public:
  using Type = T;

 public:
  Optional();
  explicit Optional(const T& value);
  explicit Optional(T&& value);

  // Copy semantics
  Optional(const Optional&) = default;
  Optional& operator=(const Optional&) = default;
  Optional& operator=(const T& other);
  Optional& operator=(T&& other);

  // Comparison
  template <class U>
  friend bool operator==(const Optional<U>& lhs, const Optional<U>& rhs);
  template <class U>
  friend bool operator!=(const Optional<U>& lhs, const Optional<U>& rhs);
  template <class U>
  friend bool operator==(const Optional<U>& lhs, const U& rhs);
  template <class U>
  friend bool operator!=(const Optional<U>& lhs, const U& rhs);
  template <class U>
  friend bool operator==(const U& lhs, const Optional<U>& rhs);
  template <class U>
  friend bool operator!=(const U& lhs, const Optional<U>& rhs);

  // Modifiers
  bool exists() const { return exists_; }
  template <class... Args>
  void emplace(Args&&... args);

  // Value access
  const T& operator*() const;
  T& operator*();
  const T * operator->() const;
  T * operator->();
  operator const T&() const;
  operator T&();

 private:
  bool exists_;
  T value_;
};

// MARK: -

template <class T>
inline Optional<T>::Optional() : exists_(), value_() {}

template <class T>
inline Optional<T>::Optional(const T& value) : exists_(true), value_(value) {}

template <class T>
inline Optional<T>::Optional(T&& value)
    : exists_(true),
      value_(std::forward<T>(value)) {}

// MARK: Copy semantics

template <class T>
inline Optional<T>& Optional<T>::operator=(const T& other) {
  exists_ = true;
  value_ = other;
  return *this;
}

template <class T>
inline Optional<T>& Optional<T>::operator=(T&& other) {
  exists_ = true;
  value_ = std::forward<T>(other);
  return *this;
}

// MARK: Comparison

template <class T>
inline bool operator==(const Optional<T>& lhs, const Optional<T>& rhs) {
  return lhs.exists_ == rhs.exists_ && lhs.value_ == rhs.value_;
}

template <class T>
inline bool operator!=(const Optional<T>& lhs, const Optional<T>& rhs) {
  return !(lhs == rhs);
}

template <class T>
inline bool operator==(const Optional<T>& lhs, const T& rhs) {
  return !lhs.exists_ && lhs.value_ == rhs.other;
}

template <class T>
inline bool operator!=(const Optional<T>& lhs, const T& rhs) {
  return !(lhs == rhs);
}

template <class T>
inline bool operator==(const T& lhs, const Optional<T>& rhs) {
  return rhs == lhs;
}

template <class T>
inline bool operator!=(const T& lhs, const Optional<T>& rhs) {
  return rhs != lhs;
}

// MARK: Modifiers

template <class T>
template <class... Args>
inline void Optional<T>::emplace(Args&&... args) {
  exists_ = true;
  value_ = T(std::forward<Args>(args)...);
}

// MARK: Value access

template <class T>
inline const T& Optional<T>::operator*() const {
  assert(exists_);
  return value_;
}

template <class T>
inline T& Optional<T>::operator*() {
  assert(exists_);
  return value_;
}

template <class T>
inline const T * Optional<T>::operator->() const {
  assert(exists_);
  return &value_;
}

template <class T>
inline T * Optional<T>::operator->() {
  assert(exists_);
  return &value_;
}

template <class T>
inline Optional<T>::operator const T&() const {
  assert(exists_);
  return value_;
}

template <class T>
inline Optional<T>::operator T&() {
  assert(exists_);
  return value_;
}

}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_OPTIONAL_H_
