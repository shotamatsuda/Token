// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#pragma once
#ifndef TOKEN_UFO_GLYPH_ITERATOR_H_
#define TOKEN_UFO_GLYPH_ITERATOR_H_

#include <cassert>
#include <functional>
#include <iterator>
#include <utility>

namespace token {
namespace ufo {

class Glyphs;

template <class T>
class GlyphIterator final : public std::iterator<std::forward_iterator_tag, T> {
 public:
  using Glyphs = typename std::conditional<
    std::is_const<T>::value,
    const Glyphs, Glyphs
  >::type;
  using Iterator = typename std::conditional<
    std::is_const<T>::value,
    typename decltype(std::declval<Glyphs>().contents_)::const_iterator,
    typename decltype(std::declval<Glyphs>().contents_)::iterator
  >::type;

 public:
  GlyphIterator();
  GlyphIterator(Glyphs *glyphs, Iterator iterator);

  // Copy semantics
  GlyphIterator(const GlyphIterator&) = default;
  GlyphIterator& operator=(const GlyphIterator&) = default;

  // Comparison
  template <class U>
  friend bool operator==(const GlyphIterator<U>& lhs,
                         const GlyphIterator<U>& rhs);
  template <class U>
  friend bool operator!=(const GlyphIterator<U>& lhs,
                         const GlyphIterator<U>& rhs);

  // Iterator
  T& operator*() const;
  T * operator->() const { return &operator*(); }
  GlyphIterator& operator++();
  GlyphIterator operator++(int);

 private:
  Glyphs *glyphs_;
  Iterator iterator_;
};

// MARK: -

template <class T>
inline GlyphIterator<T>::GlyphIterator() : glyphs_(), iterator_() {}

template <class T>
inline GlyphIterator<T>::GlyphIterator(Glyphs *glyphs, Iterator iterator)
    : glyphs_(glyphs),
      iterator_(iterator) {}

// MARK: Comparison

template <class T>
inline bool operator==(const GlyphIterator<T>& lhs,
                       const GlyphIterator<T>& rhs) {
  return lhs.glyphs_ == rhs.glyphs_ && lhs.iterator_ == rhs.iterator_;
}

template <class T>
inline bool operator!=(const GlyphIterator<T>& lhs,
                       const GlyphIterator<T>& rhs) {
  return !(lhs == rhs);
}

// MARK: Iterator

template <class T>
inline T& GlyphIterator<T>::operator*() const {
  const auto ptr = glyphs_->find(iterator_->first);
  assert(ptr);
  return *ptr;
}

template <class T>
inline GlyphIterator<T>& GlyphIterator<T>::operator++() {
  ++iterator_;
  return *this;
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
