//
//  token/ufo/glyph_iterator.h
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
#ifndef TOKEN_UFO_GLYPH_ITERATOR_H_
#define TOKEN_UFO_GLYPH_ITERATOR_H_

#include <functional>
#include <iterator>
#include <string>

namespace token {
namespace ufo {

class Glyphs;

template <class T>
class GlyphIterator final : public std::iterator<std::forward_iterator_tag, T> {
 public:
  using Glyphs = typename std::conditional<
      std::is_const<T>::value, const Glyphs, Glyphs>::type;

 public:
  GlyphIterator();
  explicit GlyphIterator(Glyphs *glyphs);

  // Copy semantics
  GlyphIterator(const GlyphIterator&) = default;
  GlyphIterator& operator=(const GlyphIterator&) = default;

  // Comparison
  bool operator==(const GlyphIterator& other) const;
  bool operator!=(const GlyphIterator& other) const;

  // Iterator
  T& operator*() const;
  T * operator->() const { return &operator*(); }
  GlyphIterator& operator++();
  GlyphIterator operator++(int);

 private:
  Glyphs *glyphs_;
  void *current_;
  std::string key_;
};

#pragma mark -

template <class T>
inline GlyphIterator<T>::GlyphIterator() : glyphs_(), current_() {}

#pragma mark Comparison

template <class T>
inline bool GlyphIterator<T>::operator==(const GlyphIterator& other) const {
  return current_ == other.current_;
}

template <class T>
inline bool GlyphIterator<T>::operator!=(const GlyphIterator& other) const {
  return !operator==(other);
}

#pragma mark Iterator

template <class T>
inline T& GlyphIterator<T>::operator*() const {
  return glyphs_->get(key_);
}

template <class T>
inline GlyphIterator<T> GlyphIterator<T>::operator++(int) {
  GlyphIterator result(*this);
  operator++();
  return result;
}

}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_GLYPH_ITERATOR_H_
