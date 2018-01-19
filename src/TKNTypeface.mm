// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

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
      try {
        subtask();
      } catch (const std::exception& e) {
        // TODO: Deal with error
        return;
      }
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
  bool result = false;
  try {
    result = token::afdko::transformFont(
        toolsPath, fontPath,
        [UPEM](boost::property_tree::ptree& tree) {
      tree.put("ttFont.head.unitsPerEm.<xmlattr>.value", UPEM);
      const auto scale = boost::lexical_cast<std::string>(1.0 / UPEM);
      const auto fontMatrix = scale + " 0 0 " + scale + " 0 0";
      tree.put("ttFont.CFF.CFFFont.FontMatrix.<xmlattr>.value", fontMatrix);
    });
  } catch (const std::exception& e) {
    // TODO: Deal with error
  }
  return result;
}

@end
