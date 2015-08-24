//
//  token/ufo/number.h
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
#ifndef TOKEN_UFO_NUMBER_H_
#define TOKEN_UFO_NUMBER_H_

#include <cassert>
#include <functional>
#include <typeinfo>
#include <type_traits>
#include <utility>

#include <boost/any.hpp>

namespace token {
namespace ufo {

class Number final {
 public:
  using IntType = int;
  using FloatType = float;

 private:
  template <typename T>
  using EnableIfInt = std::enable_if_t<std::is_integral<T>::value>;
  template <typename T>
  using EnableIfFloat = std::enable_if_t<std::is_floating_point<T>::value>;

 public:
  Number() = default;

  // Implicit conversions
  template <typename T>
  Number(T value, EnableIfInt<T> * = nullptr);
  template <typename T>
  Number(T value, EnableIfFloat<T> * = nullptr);

  // Copy semantics
  Number(const Number& other) = default;
  Number& operator=(const Number& other) = default;

  // Factory
  static Number Int();
  static Number Float();

  // Comparison
  bool operator==(const Number& other) const;
  bool operator!=(const Number& other) const;
  bool operator>(const Number& other) const;
  bool operator<(const Number& other) const;
  bool operator>=(const Number& other) const;
  bool operator<=(const Number& other) const;

  // Arithmetic
  Number& operator+=(const Number& other);
  Number& operator-=(const Number& other);
  Number& operator*=(const Number& other);
  Number& operator/=(const Number& other);
  Number operator+(const Number& other) const;
  Number operator-(const Number& other) const;
  Number operator*(const Number& other) const;
  Number operator/(const Number& other) const;
  Number operator-() const;
  Number& operator++();
  Number operator++(int);
  Number& operator--();
  Number operator--(int);

  // Attributes
  const std::type_info& type() const;
  bool empty() const;
  template <typename T>
  T as() const;

 private:
  // Templated invocation
  template <typename R, template <typename> class F>
  static R invoke(const Number& a);
  template <typename R, template <typename> class F>
  static R invoke(const Number& a, const Number& b);

 private:
  boost::any value_;
};

#pragma mark -

template <typename T>
inline Number::Number(T value, EnableIfInt<T> *)
    : value_(static_cast<IntType>(value)) {}

template <typename T>
inline Number::Number(T value, EnableIfFloat<T> *)
    : value_(static_cast<FloatType>(value)) {}

#pragma mark Factory

inline Number Number::Int() {
  return Number(IntType());
}

inline Number Number::Float() {
  return Number(FloatType());
}

#pragma mark Comparison

inline bool Number::operator==(const Number& other) const {
  if (empty() || other.empty()) {
    return empty() == other.empty();
  }
  return invoke<bool, std::equal_to>(*this, other);
}

inline bool Number::operator!=(const Number& other) const {
  return !operator==(other);
}

inline bool Number::operator>(const Number& other) const {
  if (empty() || other.empty()) {
    return false;
  }
  return invoke<bool, std::greater>(*this, other);
}

inline bool Number::operator<(const Number& other) const {
  if (empty() || other.empty()) {
    return false;
  }
  return invoke<bool, std::less>(*this, other);
}

inline bool Number::operator>=(const Number& other) const {
  if (empty() || other.empty()) {
    return false;
  }
  return invoke<bool, std::greater_equal>(*this, other);
}

inline bool Number::operator<=(const Number& other) const {
  if (empty() || other.empty()) {
    return false;
  }
  return invoke<bool, std::less_equal>(*this, other);
}

#pragma mark Arithmetic

inline Number& Number::operator+=(const Number& other) {
  if (empty() || other.empty()) {
    return *this;
  }
  return *this = invoke<Number, std::plus>(*this, other);
}

inline Number& Number::operator-=(const Number& other) {
  if (empty() || other.empty()) {
    return *this;
  }
  return *this = invoke<Number, std::minus>(*this, other);
}

inline Number& Number::operator*=(const Number& other) {
  if (empty() || other.empty()) {
    return *this;
  }
  return *this = invoke<Number, std::multiplies>(*this, other);
}

inline Number& Number::operator/=(const Number& other) {
  if (empty() || other.empty()) {
    return *this;
  }
  return *this = invoke<Number, std::divides>(*this, other);
}

inline Number Number::operator+(const Number& other) const {
  if (empty() || other.empty()) {
    return Number();
  }
  return invoke<Number, std::plus>(*this, other);
}

inline Number Number::operator-(const Number& other) const {
  if (empty() || other.empty()) {
    return Number();
  }
  return invoke<Number, std::minus>(*this, other);
}

inline Number Number::operator*(const Number& other) const {
  if (empty() || other.empty()) {
    return Number();
  }
  return invoke<Number, std::multiplies>(*this, other);
}

inline Number Number::operator/(const Number& other) const {
  if (empty() || other.empty()) {
    return Number();
  }
  return invoke<Number, std::divides>(*this, other);
}

inline Number Number::operator-() const {
  if (empty()) {
    return Number();
  }
  return invoke<Number, std::negate>(*this);
}

inline Number& Number::operator++() {
  if (empty()) {
    return *this;
  }
  return *this += Number(1);
}

inline Number Number::operator++(int) {
  if (empty()) {
    return Number();
  }
  const Number result(*this);
  operator++();
  return std::move(result);
}

inline Number& Number::operator--() {
  if (empty()) {
    return *this;
  }
  return *this -= Number(1);
}

inline Number Number::operator--(int) {
  if (empty()) {
    return Number();
  }
  const Number result(*this);
  operator--();
  return std::move(result);
}

#pragma mark Value retrieval

inline const std::type_info& Number::type() const {
  return value_.type();
}

inline bool Number::empty() const {
  return value_.empty();
}

template <typename T>
inline T Number::as() const {
  if (empty()) {
    return T();
  } else if (type() == typeid(IntType)) {
    return boost::any_cast<IntType>(value_);
  } else if (type() == typeid(FloatType)) {
    return boost::any_cast<FloatType>(value_);
  }
  assert(false);
  return T();
}

#pragma mark Templated invocation

template <typename R, template <typename> class F>
inline R Number::invoke(const Number& a) {
  assert(!a.empty());
  if (a.type() == typeid(IntType)) {
    return R(F<IntType>()(a.as<IntType>()));
  } else if (a.type() == typeid(FloatType)) {
    return R(F<FloatType>()(a.as<FloatType>()));
  }
  assert(false);
  return R();
}

template <typename R, template <typename> class F>
inline R Number::invoke(const Number& a, const Number& b) {
  assert(!a.empty());
  assert(!b.empty());
  if (a.type() == typeid(IntType)) {
    if (b.type() == typeid(IntType)) {
      return R(F<IntType>()(a.as<IntType>(), b.as<IntType>()));
    } else if (b.type() == typeid(FloatType)) {
      return R(F<FloatType>()(a.as<IntType>(), b.as<FloatType>()));
    }
  } else if (a.type() == typeid(FloatType)) {
    if (b.type() == typeid(IntType)) {
      return R(F<FloatType>()(a.as<FloatType>(), b.as<IntType>()));
    } else if (b.type() == typeid(FloatType)) {
      return R(F<FloatType>()(a.as<FloatType>(), b.as<FloatType>()));
    }
  }
  assert(false);
  return R();
}

}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_NUMBER_H_
