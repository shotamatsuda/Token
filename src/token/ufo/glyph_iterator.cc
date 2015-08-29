//
//  token/ufo/glyph_iterator.cc
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

#include "token/ufo/glyph_iterator.h"

extern "C" {

#include <plist/plist.h>

}  // extern "C"

#include <cassert>
#include <cstdlib>

#include "token/ufo/glif/glyph.h"
#include "token/ufo/glyphs.h"

namespace token {
namespace ufo {

template <class T>
GlyphIterator<T>::GlyphIterator(Glyphs *glyphs)
    : glyphs_(glyphs),
      current_() {
  if (glyphs_->contents_) {
    assert(plist_get_node_type(glyphs_->contents_) == PLIST_DICT);
    plist_dict_new_iter(glyphs_->contents_, &current_);
    operator++();
  }
}

#pragma mark Iterator

template <class T>
inline GlyphIterator<T>& GlyphIterator<T>::operator++() {
  char *key;
  plist_t value{};
  plist_dict_next_item(glyphs_->contents_, current_, &key, &value);
  if (key) {
    key_ = key;
    std::free(key);
  } else {
    key_.clear();
    current_ = nullptr;
  }
  return *this;
}

template class GlyphIterator<Glyph>;
template class GlyphIterator<const Glyph>;

}  // namespace ufo
}  // namespace token
