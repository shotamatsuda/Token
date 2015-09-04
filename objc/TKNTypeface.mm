//
//  TKNTypeface.mm
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

#define TAKRAM_HAS_COREGRAPHICS 1

#import "TKNTypeface.h"

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstdlib>
#include <iterator>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <boost/filesystem/path.hpp>

#include "takram/graphics.h"
#include "takram/math.h"
#include "token/glyph_outline.h"
#include "token/glyph_stroker.h"
#include "token/ufo.h"

static const double kTKNTypefaceStrokingRetryShift = 0.001;
static const double kTKNTypefaceStrokingRetryShiftLimit = 0.1;
static const double kTKNTypefaceMinStrokeWidth = 10.0;
static const double kTKNTypefaceMaxStrokeWidth = 120.0;

@interface TKNTypeface () {
 @private
  token::ufo::FontInfo _fontInfo;
  token::ufo::Glyphs _glyphs;
  std::unordered_map<std::string, token::GlyphOutline> _glyphOutlines;
  std::unordered_map<std::string, takram::Shape2d> _glyphShapes;
  double _strokeWidth;
}

@property (nonatomic, strong) NSMutableDictionary *glyphBezierPaths;
@property (nonatomic, assign, readonly) double strokeWidth;

#pragma mark Opening and Saving

- (NSString *)createUnifiedFontObject:(NSString *)directory;
- (void)updateGlyphsInUnifiedFontObject:(NSString *)path;
- (NSString *)createOpenTypeWithUnifiedFontObject:(NSString *)path;

@end

@implementation TKNTypeface

- (instancetype)init {
  return [self initWithFileAtPath:nil];
}

- (instancetype)initWithFileAtPath:(NSString *)path {
  self = [super init];
  if (self) {
    _glyphBezierPaths = [NSMutableDictionary dictionary];
    _capHeight = 2.0;
    _width = 0.2;
    if (path) {
      [self openFile:path];
    }
  }
  return self;
}

#pragma mark Opening and Saving

- (void)openFile:(NSString *)path {
  _path = path;
  _fontInfo.open(_path.UTF8String);
  _glyphs.open(_path.UTF8String);
  for (auto& glyph : _glyphs) {
    _glyphOutlines.emplace(glyph.name, token::GlyphOutline(glyph));
  }
}

- (void)saveToFile:(NSString *)path {
  NSString *uniqueString = [NSProcessInfo processInfo].globallyUniqueString;
  NSString *workingDirectoryPath = [NSTemporaryDirectory()
      stringByAppendingPathComponent:uniqueString];
  NSString *ufoPath = [self createUnifiedFontObject:workingDirectoryPath];
  [self updateGlyphsInUnifiedFontObject:ufoPath];
  NSString *otfPath = [self createOpenTypeWithUnifiedFontObject:ufoPath];
  NSFileManager *fileManager = [NSFileManager defaultManager];
  [fileManager copyItemAtPath:otfPath toPath:path error:NULL];
  [fileManager removeItemAtPath:workingDirectoryPath error:NULL];
}

- (NSString *)createUnifiedFontObject:(NSString *)directory {
  NSString *ufoPath = [directory stringByAppendingPathComponent:
      [[_path.lastPathComponent stringByDeletingPathExtension]
          stringByAppendingPathExtension:@"ufo"]];
  NSFileManager *fileManager = [NSFileManager defaultManager];
  [fileManager createDirectoryAtPath:directory
         withIntermediateDirectories:YES
                          attributes:nil
                               error:NULL];
  [fileManager copyItemAtPath:_path toPath:ufoPath error:NULL];
  return ufoPath;
}

- (void)updateGlyphsInUnifiedFontObject:(NSString *)path {
  const auto glyphsPath = boost::filesystem::path(path.UTF8String) / "glyphs";
  for (const auto& glyph : _glyphs) {
    assert(_glyphOutlines.find(glyph.name) != std::end(_glyphOutlines));
    auto outline = _glyphOutlines.at(glyph.name);
    outline.shape() = [self strokeGlyph:glyph outline:outline];
    const auto glyphPath = glyphsPath / _glyphs.filename(glyph.name);
    outline.glyph(glyph).save(glyphPath.string());
  }
}

- (NSString *)createOpenTypeWithUnifiedFontObject:(NSString *)path {
  NSString *directory = [path stringByDeletingLastPathComponent];
  NSString *otfPath = [directory stringByAppendingPathComponent:
      [[_path.lastPathComponent stringByDeletingPathExtension]
          stringByAppendingPathExtension:@"otf"]];
  const auto sharedSupportPath = boost::filesystem::path(
      [NSBundle mainBundle].sharedSupportPath.UTF8String);
  const auto toolsPath = sharedSupportPath / "FDK" / "Tools" / "osx";
  const std::string makeotf = (toolsPath / "makeotf").string();
  std::system(("FDK_EXE=\"" + toolsPath.string() + "\";"
               "PATH=${PATH}:\"" + toolsPath.string() + "\";"
               "export PATH;"
               "export FDK_EXE;" +
               makeotf + " -f \"" + path.UTF8String + "\"" +
               " -o \"" + otfPath.UTF8String + "\"").c_str());
  return otfPath;
}

#pragma mark Parameters

- (void)setCapHeight:(double)capHeight {
  if (capHeight != _capHeight) {
    _capHeight = capHeight;
    _strokeWidth = 0.0;
    _glyphShapes.clear();
    [_glyphBezierPaths removeAllObjects];
  }
}

- (void)setWidth:(double)width {
  if (width != _width) {
    _width = width;
    _strokeWidth = 0.0;
    _glyphShapes.clear();
    [_glyphBezierPaths removeAllObjects];
  }
}

- (double)strokeWidth {
  if (!_strokeWidth) {
    _strokeWidth = takram::math::clamp(
        std::round((_width * _fontInfo.cap_height) / _capHeight),
        kTKNTypefaceMinStrokeWidth, kTKNTypefaceMaxStrokeWidth);
  }
  return _strokeWidth;
}

#pragma mark Typographic Properties

- (NSString *)familyName {
  return [NSString stringWithUTF8String:_fontInfo.family_name.c_str()];
}

- (NSString *)styleName {
  return [NSString stringWithUTF8String:_fontInfo.style_name.c_str()];
}

- (NSNumber *)proposedSize {
  return nil;
}

- (NSUInteger)unitsPerEM {
  return _fontInfo.units_per_em;
}

- (NSInteger)ascender {
  return _fontInfo.ascender;
}

- (NSInteger)descender {
  return _fontInfo.descender;
}

- (NSString *)postscriptFontName {
  return [NSString stringWithUTF8String:_fontInfo.postscript_font_name.c_str()];
}

#pragma mark Glyphs

- (NSBezierPath *)glyphOutlineForName:(NSString *)name {
  const auto glyph = _glyphs.find(name.UTF8String);
  if (!glyph) {
    return nil;
  }
  NSBezierPath *path = _glyphBezierPaths[name];
  if (path) {
    return path;
  }
  assert(_glyphOutlines.find(name.UTF8String) != std::end(_glyphOutlines));
  assert(_glyphShapes.find(name.UTF8String) == std::end(_glyphShapes));
  auto outline = _glyphOutlines.at(name.UTF8String);
  auto shape = [self strokeGlyph:*glyph outline:outline];
  path = [self bezierPathWithShape:shape];
  _glyphShapes.emplace(name.UTF8String, std::move(shape));
  _glyphBezierPaths[name] = path;
  return path;
}

- (double)advanceOfGlyphForName:(NSString *)name {
  const auto glyph = _glyphs.find(name.UTF8String);
  if (!glyph) {
    return 0.0;
  }
  return glyph->advance->width;
}

- (NSBezierPath *)bezierPathWithShape:(const takram::Shape2d&)shape {
  NSBezierPath *path = [NSBezierPath bezierPath];
  for (const auto& command : shape) {
    switch (command.type()) {
      case takram::graphics::CommandType::MOVE:
        [path moveToPoint:command.point()];
        break;
      case takram::graphics::CommandType::LINE:
        [path lineToPoint:command.point()];
        break;
      case takram::graphics::CommandType::CUBIC:
        [path curveToPoint:command.point()
             controlPoint1:command.control1()
             controlPoint2:command.control2()];
        break;
      case takram::graphics::CommandType::CLOSE:
        [path closePath];
        break;
      default:
        assert(false);
        break;
    }
  }
  return path;
}

- (takram::Shape2d)strokeGlyph:(const token::ufo::Glyph&)glyph
                       outline:(const token::GlyphOutline&)outline {
  const auto width = self.strokeWidth;
  const auto scale = (_fontInfo.cap_height - width) / _fontInfo.cap_height;
  const takram::Vec2d center(glyph.advance->width / 2.0,
                             _fontInfo.cap_height / 2.0);
  auto stroked = outline;
  for (auto& command : stroked.shape()) {
    command.point() = center + (command.point() - center) * scale;
    command.control1() = center + (command.control1() - center) * scale;
    command.control2() = center + (command.control2() - center) * scale;
  }

  // Stroking and path simplification
  token::GlyphStroker stroker;
  stroker.set_width(width);
  takram::Shape2d shape;
  // Because the path simplification occationally fails
  bool success{};
  for (double shift{};
       shift < kTKNTypefaceStrokingRetryShiftLimit;
       shift += kTKNTypefaceStrokingRetryShift) {
    stroker.set_width(width + shift);
    shape = stroker.stroke(stroked);
    shape = stroker.simplify(shape);
    if (shape.size() == glyph.lib->number_of_contours) {
      success = true;
      break;
    }
  }
  if (success) {
    shape.convertConicsToQuadratics();
    shape.convertQuadraticsToCubics();
    shape.removeDuplicates(1.0);
  } else {
    shape.reset();
  }
  return std::move(shape);
}

@end
