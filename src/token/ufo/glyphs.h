//
//  token/ufo/glyphs.h
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

 public:
  Glyphs() = default;
  explicit Glyphs(const std::string& path);

  // Disallow copy semantics
  Glyphs(const Glyphs&) = delete;
  Glyphs& operator=(const Glyphs&) = delete;

  // Move semantics
  Glyphs(Glyphs&& other) = default;
  Glyphs& operator=(Glyphs&& other) = default;

  // Opening
  bool open(const std::string& path);

  // Glyphs
  std::string filename(const std::string& name) const;
  const Glyph& get(const std::string& name) const;
  Glyph& get(const std::string& name);
  const Glyph * find(const std::string& name) const;
  Glyph * find(const std::string& name);
  void set(const std::string& name, const Glyph& glyph);

  // Iterator
  Iterator begin();
  ConstIterator begin() const;
  Iterator end();
  ConstIterator end() const;

 private:
  bool open(std::istream *stream);

 private:
  std::string path_;
  std::vector<std::pair<std::string, std::string>> contents_;
  mutable std::vector<std::pair<std::string, Glyph>> glyphs_;

 private:
  template <class T>
  friend class GlyphIterator;
};

#pragma mark -

inline Glyphs::Glyphs(const std::string& path) {
  open(path);
}

#pragma mark Glyphs

inline const Glyph& Glyphs::get(const std::string& name) const {
  auto glyph = find(name);
  assert(glyph);
  return *glyph;
}

inline Glyph& Glyphs::get(const std::string& name) {
  auto glyph = find(name);
  assert(glyph);
  return *glyph;
}

#pragma mark Iterator

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
