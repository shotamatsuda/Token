//
//  token/ufo/glyphs.h
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
#ifndef TOKEN_UFO_GLYPHS_H_
#define TOKEN_UFO_GLYPHS_H_

#include <fstream>
#include <string>
#include <unordered_map>
#include <utility>

#include "token/ufo/glyph.h"
#include "token/ufo/glyph_iterator.h"

namespace token {
namespace ufo {

class Glyphs final {
 public:
  using Iterator = GlyphIterator<Glyph>;
  using ConstIterator = GlyphIterator<const Glyph>;

 public:
  Glyphs();
  explicit Glyphs(const std::string& path);
  ~Glyphs();

  // Disallow copy semantics
  Glyphs(const Glyphs&) = delete;
  Glyphs& operator=(const Glyphs&) = delete;

  // Move semantics
  Glyphs(Glyphs&& other);
  Glyphs& operator=(Glyphs&& other);

  // Glyphs
  const Glyph& get(const std::string& name) const;
  Glyph& get(const std::string& name);
  const Glyph * find(const std::string& name) const;
  Glyph * find(const std::string& name);
  void set(const std::string& name, const Glyph& glyph);

  // Iterator
  Iterator begin() { return Iterator(this); }
  ConstIterator begin() const { return ConstIterator(this); }
  Iterator end() { return Iterator(); }
  ConstIterator end() const { return ConstIterator(); }

 private:
  void * openPropertyList(const std::string& file) const;
  std::ifstream openGLIF(const std::string& name) const;
  Glyph readGlyph(std::ifstream *stream) const;

 private:
  std::string path_;
  void *contents_;
  void *layerinfo_;
  mutable std::unordered_map<std::string, Glyph> glyphs_;

 private:
  template <class T>
  friend class GlyphIterator;
};

#pragma mark -

inline Glyphs::Glyphs() : contents_(), layerinfo_() {}

inline Glyphs::Glyphs(const std::string& path)
    : path_(path),
      contents_(openPropertyList("contents.plist")),
      layerinfo_(openPropertyList("layerinfo.plist")) {}

#pragma mark Move semantics

inline Glyphs::Glyphs(Glyphs&& other)
    : path_(std::move(other.path_)),
      contents_(other.contents_),
      layerinfo_(other.layerinfo_),
      glyphs_(std::move(other.glyphs_)) {
  other.contents_ = nullptr;
  other.layerinfo_ = nullptr;
}

inline Glyphs& Glyphs::operator=(Glyphs&& other) {
  if (&other != this) {
    std::swap(path_, other.path_);
    std::swap(contents_, other.contents_);
    std::swap(layerinfo_, other.layerinfo_);
    std::swap(glyphs_, other.glyphs_);
  }
  return *this;
}

}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_GLYPHS_H_
