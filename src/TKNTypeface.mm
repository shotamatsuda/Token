//
//  TKNTypeface.mm
//
//  The MIT License
//
//  Copyright (C) 2015-2017 Shota Matsuda
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

#include <fstream>
#include <sstream>
#include <string>

#include <boost/algorithm/string/replace.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "token/afdko.h"
#include "token/ufo.h"

@implementation TKNTypeface

- (void)createFontWithContentsOfURL:(NSURL *)contentsURL
                              toURL:(NSURL *)fontURL
                           toolsURL:(NSURL *)toolsURL
                           extraURL:(NSURL *)extraURL
                    progressHandler:(TKNTypefaceProgressHandler)progressHandler
                  completionHandler:(dispatch_block_t)completionHandler {
  const std::string directoryPath(
      fontURL.URLByDeletingLastPathComponent.path.UTF8String);
  const std::string contentsPath(contentsURL.path.UTF8String);
  const std::string fontPath(fontURL.path.UTF8String);
  const std::string toolsPath(toolsURL.path.UTF8String);
  const std::string extraPath(extraURL.path.UTF8String);
  const token::ufo::FontInfo fontInfo(contentsPath);
  const token::ufo::Glyphs glyphs(contentsPath);
  NSArray *subtasks = @[
    ^{ token::afdko::checkOutlines(toolsPath, contentsPath); },
    ^{ token::afdko::performAutoHinting(toolsPath, contentsPath); },
    ^{ token::afdko::createFeatures(fontInfo, directoryPath); },
    ^{ token::afdko::createFontMenuName(fontInfo, directoryPath); },
    ^{ token::afdko::createGlyphOrderAndAlias(glyphs, directoryPath); },
    ^{ token::afdko::generateKernFile(extraPath, contentsPath); },
    ^{ token::afdko::createOpenTypeFont(toolsPath, contentsPath, fontPath); }
  ];
  dispatch_async(dispatch_get_global_queue(
      DISPATCH_QUEUE_PRIORITY_BACKGROUND, 0), ^{
    NSUInteger numberOfSubtasks{};
    for (dispatch_block_t subtask in subtasks) {
      subtask();
      ++numberOfSubtasks;
      if (progressHandler) {
        dispatch_async(dispatch_get_main_queue(), ^{
          progressHandler(numberOfSubtasks, subtasks.count);
        });
      }
    }
    if (completionHandler) {
      dispatch_async(dispatch_get_main_queue(), ^{
        completionHandler();
      });
    }
  });
}

- (BOOL)correctUPEM:(double)UPEM
       forFontAtURL:(NSURL *)fontURL
           toolsURL:(NSURL *)toolsURL {
  const std::string fontPath(fontURL.path.UTF8String);
  const std::string toolsPath(toolsURL.path.UTF8String);
  return token::afdko::transformFont(
      toolsPath, fontPath,
      [UPEM](boost::property_tree::ptree& tree) {
    tree.put("ttFont.head.unitsPerEm.<xmlattr>.value", UPEM);
    const auto scale = boost::lexical_cast<std::string>(1.0 / UPEM);
    const auto fontMatrix = scale + " 0 0 " + scale + " 0 0";
    tree.put("ttFont.CFF.CFFFont.FontMatrix.<xmlattr>.value", fontMatrix);
  });
}

@end
