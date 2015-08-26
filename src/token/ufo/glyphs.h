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

#include <memory>
#include <string>
#include <unordered_map>

#include "token/ufo/glif/glyph.h"

namespace token {
namespace ufo {

class Glyphs final {
 public:
  Glyphs();
  explicit Glyphs(const std::string& path);

  // Disallow copy semantics
  Glyphs(const Glyphs&) = delete;
  Glyphs& operator=(const Glyphs&) = delete;

  // Move semantics
  Glyphs(Glyphs&&) = default;
  Glyphs& operator=(Glyphs&&) = default;

  // Glyphs
  const std::unique_ptr<Glyph>& get(const std::string& name) const;

 private:
  void * open(const std::string& file_name) const;
  std::unique_ptr<Glyph> read(const std::string& name) const;

 private:
  std::string path_;
  void *contents_;
  void *layerinfo_;
  mutable std::unordered_map<std::string, std::unique_ptr<Glyph>> glyphs_;
};

#pragma mark -

inline Glyphs::Glyphs() : contents_(), layerinfo_() {}

inline Glyphs::Glyphs(const std::string& path)
    : path_(path),
      contents_(open("contents.plist")),
      layerinfo_(open("layerinfo.plist")) {}

}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_GLYPHS_H_
