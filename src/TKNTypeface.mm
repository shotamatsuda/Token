//
//  TKNTypeface.mm
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

#import "TKNTypeface.h"

#include <cassert>
#include <string>

#include "sfntly/font.h"
#include "sfntly/port/type.h"
#include "sfntly/table/core/font_header_table.h"
#include "sfntly/tag.h"

#include "token/afdko.h"
#include "token/ufo.h"
#include "token/sfnt.h"

@implementation TKNTypeface

- (BOOL)createFontWithContentsOfURL:(NSURL *)contentsURL
                              toURL:(NSURL *)fontURL
                           toolsURL:(NSURL *)toolsURL
                           extraURL:(NSURL *)extraURL
                              error:(NSError **)error {
  const std::string directoryPath(
      fontURL.URLByDeletingLastPathComponent.path.UTF8String);
  const std::string contentsPath(contentsURL.path.UTF8String);
  const std::string fontPath(fontURL.path.UTF8String);
  const std::string toolsPath(toolsURL.path.UTF8String);
  const std::string extraPath(extraURL.path.UTF8String);
  const token::ufo::FontInfo fontInfo(contentsPath);
  const token::ufo::Glyphs glyphs(contentsPath);
  token::afdko::checkOutlines(toolsPath, contentsPath);
  token::afdko::autohint(toolsPath, contentsPath, true);
  token::afdko::createFeatures(fontInfo, directoryPath);
  token::afdko::createFontMenuNameDB(fontInfo, directoryPath);
  token::afdko::createGlyphOrderAndAliasDB(glyphs, directoryPath);
  token::afdko::generateKernFile(extraPath, contentsPath);
  token::afdko::makeotf(toolsPath, contentsPath, fontPath, true);
  return true;
}

- (BOOL)correctUPEM:(double)UPEM forFontAtURL:(NSURL *)URL {
  const std::string path(URL.path.UTF8String);
  sfntly::FontBuilderPtr fontBuilder;
  fontBuilder.Attach(token::sfnt::loadFontBuilder(path));
  const auto tableBuilder = down_cast<sfntly::FontHeaderTable::Builder *>(
      fontBuilder->GetTableBuilder(sfntly::Tag::head));
  if (tableBuilder->UnitsPerEm() == UPEM) {
    return YES;
  }
  tableBuilder->SetUnitsPerEm(UPEM);
  if (!fontBuilder->ReadyToBuild()) {
    return NO;
  }
  sfntly::FontPtr font;
  font.Attach(fontBuilder->Build());
  token::sfnt::serializeFont(path, font);
  font.Attach(token::sfnt::loadFont(path));
  const auto table = down_cast<sfntly::FontHeaderTable *>(
      font->GetTable(sfntly::Tag::head));
  return table->UnitsPerEm() == UPEM;
}

@end
