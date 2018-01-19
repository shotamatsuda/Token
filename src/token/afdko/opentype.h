// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

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
