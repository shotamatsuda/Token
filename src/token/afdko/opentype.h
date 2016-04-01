//
//  token/afdko/opentype.h
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
#ifndef TOKEN_AFDKO_OPENTYPE_H_
#define TOKEN_AFDKO_OPENTYPE_H_

#include <string>

#include "token/ufo.h"

namespace token {
namespace afdko {

bool createOpenTypeFont(const std::string& directory,
                        const std::string& input,
                        const std::string& output);
void createFeatures(const ufo::FontInfo& font_info,
                    const std::string& output);
void createFontMenuName(const ufo::FontInfo& font_info,
                        const std::string& output);
void createGlyphOrderAndAlias(const ufo::Glyphs& glyphs,
                              const std::string& output);

}  // namespace afdko
}  // namespace token

#endif  // TOKEN_AFDKO_OPENTYPE_H_