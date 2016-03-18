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

#include <string>

#include "token/afdko.h"
#include "token/ufo.h"

@implementation TKNTypeface

- (BOOL)createFontWithContentsOfURL:(NSURL *)contentsURL
                              toURL:(NSURL *)fontURL
                           toolsURL:(NSURL *)toolsURL
                              error:(NSError **)error {
  const std::string fontPath(fontURL.path.UTF8String);
  const std::string directoryPath(
      fontURL.URLByDeletingLastPathComponent.path.UTF8String);
  const std::string contentsPath(contentsURL.path.UTF8String);
  const std::string toolsPath(toolsURL.path.UTF8String);
  const token::ufo::FontInfo fontInfo(contentsPath);
  const token::ufo::Glyphs glyphs(contentsPath);
  token::afdko::checkOutlines(toolsPath, contentsPath);
  token::afdko::autohint(toolsPath, contentsPath, true);
  token::afdko::createFeatures(fontInfo, directoryPath);
  token::afdko::createFontMenuNameDB(fontInfo, directoryPath);
  token::afdko::createGlyphOrderAndAliasDB(glyphs, directoryPath);
  return token::afdko::makeotf(toolsPath, contentsPath, fontPath, true);
}

@end
