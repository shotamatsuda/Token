// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#pragma once
#ifndef TOKEN_UFO_GLYPHS_H_
#define TOKEN_UFO_GLYPHS_H_

#include <fstream>
#include <string>
#include <iterator>
#include <utility>
#include <vector>

#include "token/ufo/glyph.h"
#include "token/ufo/glyph_iterator.h"

namespace token {
namespace ufo {

class Glyphs final {
 public:
  using Iterator = GlyphIterator<Glyph>;
  using ConstIterator = GlyphIterator<const Glyph>;

 private:
  template <class T>
  friend class GlyphIterator;

 public:
  Glyphs() = default;
  explicit Glyphs(const std::string& path);

  // Copy semantics
  Glyphs(const Glyphs&) = default;
  Glyphs& operator=(const Glyphs&) = default;

  // Move semantics
  Glyphs(Glyphs&& other) = default;
  Glyphs& operator=(Glyphs&& other) = default;

  // Opening
  bool open(const std::string& path);

  // Glyphs
  std::string filename(const std::string& name) const;
  const Glyph * find(const std::string& name) const;
  Glyph * find(const std::string& name);
  void set(const std::string& name, const Glyph& glyph);

  // Iterator
  Iterator begin();
  ConstIterator begin() const;
  Iterator end();
  ConstIterator end() const;

 private:
  bool open(std::istream& stream);

 private:
  std::string path_;
  std::vector<std::pair<std::string, std::string>> contents_;
  mutable std::vector<std::pair<std::string, Glyph>> glyphs_;
};

// MARK: -

inline Glyphs::Glyphs(const std::string& path) {
  open(path);
}

// MARK: Iterator

inline typename Glyphs::Iterator Glyphs::begin() {
  return Iterator(this, std::begin(contents_));
}

inline typename Glyphs::ConstIterator Glyphs::begin() const {
  return ConstIterator(this, std::begin(contents_));
}

inline typename Glyphs::Iterator Glyphs::end() {
  return Iterator(this, std::end(contents_));
}

inline typename Glyphs::ConstIterator Glyphs::end() const {
  return ConstIterator(this, std::end(contents_));
}

}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_GLYPHS_H_
