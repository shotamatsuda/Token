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

#include <cassert>
#include <cmath>
#include <iterator>
#include <string>
#include <unordered_map>
#include <utility>

#include <boost/lexical_cast.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/format.hpp>

#include "takram/graphics.h"
#include "takram/math.h"
#include "token/afdko.h"
#include "token/glyph_outline.h"
#include "token/glyph_stroker.h"
#include "token/ufo.h"

#import "TKNTypefaceUnit.h"

static const double kTKNTypefaceStrokingRetryShift = 0.001;
static const double kTKNTypefaceStrokingRetryShiftLimit = 0.1;
static const double kTKNTypefaceMinStrokeWidthInEM = 10.0;
static const double kTKNTypefaceMaxStrokeWidthInEM = 120.0;

@interface TKNTypeface () {
 @private
  token::ufo::FontInfo _fontInfo;
  token::ufo::Glyphs _glyphs;
  std::unordered_map<std::string, token::GlyphOutline> _glyphOutlines;
  std::unordered_map<std::string, takram::Shape2d> _glyphShapes;
  double _strokeWidthInEM;
}

@property (nonatomic, strong, nonnull) NSString *styleName;
@property (nonatomic, strong, nonnull) NSString *postscriptFontName;

@property (nonatomic, strong) NSMutableDictionary *glyphBezierPaths;
@property (nonatomic, assign, readonly) double strokeWidthInEM;

- (void)update;

#pragma mark Opening and Saving

- (NSString *)createUnifiedFontObject:(NSString *)directory;
- (void)updateFontInfoInUnifiedFontObject:(NSString *)path;
- (void)updateGlyphsInUnifiedFontObject:(NSString *)path;
- (NSString *)createOpenTypeWithUnifiedFontObject:(NSString *)path;
- (NSString *)createFontMenuNameDB:(NSString *)directory;
- (NSString *)createGlyphOrderAndAliasDB:(NSString *)directory;

@end

@implementation TKNTypeface

- (instancetype)init {
  return [self initWithFileAtPath:nil];
}

- (instancetype)initWithFileAtPath:(NSString *)path {
  if (!path) {
    return nil;
  }
  self = [super init];
  if (self) {
    _glyphBezierPaths = [NSMutableDictionary dictionary];
    if (path) {
      [self openFile:path];
    }
    self.capHeight = 2.5;
    self.width = 0.2;
    self.capHeightEqualsUnitsPerEM = YES;
  }
  return self;
}

- (void)update {
  _strokeWidthInEM = 0.0;
  _glyphShapes.clear();
  [_glyphBezierPaths removeAllObjects];
  NSNumberFormatter *formatter = [[NSNumberFormatter alloc] init];
  formatter.numberStyle = NSNumberFormatterDecimalStyle;
  formatter.minimumFractionDigits = 2;
  formatter.maximumFractionDigits = 2;
  std::string style;
  style += [formatter stringFromNumber:
      [NSNumber numberWithDouble:_width]].UTF8String;
  style += TKNTypefaceUnitAbbreviatedName(_widthUnit).UTF8String;
  style += " / ";
  style += [formatter stringFromNumber:
      [NSNumber numberWithDouble:_capHeight]].UTF8String;
  style += TKNTypefaceUnitAbbreviatedName(_capHeightUnit).UTF8String;
  std::string postscriptStyle;
  postscriptStyle += [formatter stringFromNumber:
      [NSNumber numberWithDouble:_width]].UTF8String;
  postscriptStyle += TKNTypefaceUnitAbbreviatedName(_widthUnit).UTF8String;
  postscriptStyle += "-";
  postscriptStyle += [formatter stringFromNumber:
      [NSNumber numberWithDouble:_capHeight]].UTF8String;
  postscriptStyle += TKNTypefaceUnitAbbreviatedName(_capHeightUnit).UTF8String;
  self.styleName = [NSString stringWithUTF8String:style.c_str()];
  self.postscriptFontName = [[self.familyName stringByAppendingString:@"-"]
      stringByAppendingString:
          [NSString stringWithUTF8String:postscriptStyle.c_str()]];
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
  [self updateFontInfoInUnifiedFontObject:ufoPath];
  [self updateGlyphsInUnifiedFontObject:ufoPath];
  NSString *otfPath = [self createOpenTypeWithUnifiedFontObject:ufoPath];
  NSFileManager *fileManager = [NSFileManager defaultManager];
  [fileManager removeItemAtPath:path error:NULL];
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

- (void)updateFontInfoInUnifiedFontObject:(NSString *)path {
  token::ufo::FontInfo fontInfo = _fontInfo;
  NSNumberFormatter *formatter = [[NSNumberFormatter alloc] init];
  formatter.numberStyle = NSNumberFormatterDecimalStyle;
  formatter.minimumFractionDigits = 2;
  formatter.maximumFractionDigits = 2;
  const std::string style = _styleName.UTF8String;
  const std::string postscriptFontName = _postscriptFontName.UTF8String;
  fontInfo.style_name = style;
  fontInfo.style_map_style_name = style;
  fontInfo.postscript_font_name = postscriptFontName;
  fontInfo.postscript_full_name = fontInfo.family_name + " " + style;
  fontInfo.open_type_name_preferred_subfamily_name = style;
  fontInfo.open_type_name_unique_id =
      fontInfo.open_type_name_version + ";" +
      fontInfo.open_type_os2_vendor_id + ";" +
      fontInfo.postscript_font_name;
  fontInfo.postscript_stem_snap_h.clear();
  fontInfo.postscript_stem_snap_h.emplace_back(self.strokeWidthInEM);
  fontInfo.postscript_stem_snap_v.clear();
  fontInfo.postscript_stem_snap_v.emplace_back(self.strokeWidthInEM);
  fontInfo.save(path.UTF8String);
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
  const token::ufo::FontInfo font_info(path.UTF8String);
  const token::ufo::Glyphs glyphs(path.UTF8String);
  NSString *directory = [path stringByDeletingLastPathComponent];
  NSString *otfPath = [directory stringByAppendingPathComponent:
      [[_path.lastPathComponent stringByDeletingPathExtension]
          stringByAppendingPathExtension:@"otf"]];
  const auto sharedSupportPath = boost::filesystem::path(
      [NSBundle mainBundle].sharedSupportPath.UTF8String);
  const auto toolsPath = sharedSupportPath / "FDK" / "Tools" / "osx";
  token::afdko::autohint(toolsPath.string(), path.UTF8String, true);
  token::afdko::createFeatures(font_info, directory.UTF8String);
  token::afdko::createFontMenuNameDB(font_info, directory.UTF8String);
  token::afdko::createGlyphOrderAndAliasDB(glyphs, directory.UTF8String);
  token::afdko::makeotf(toolsPath.string(),
                        path.UTF8String,
                        otfPath.UTF8String,
                        true);
  return otfPath;
}

- (NSString *)createFontMenuNameDB:(NSString *)directory {
  // TODO(shotamatsuda):
  return nil;
}

- (NSString *)createGlyphOrderAndAliasDB:(NSString *)directory {
  // TODO(shotamatsuda):
  return nil;
}

#pragma mark Parameters

- (void)setCapHeight:(double)capHeight {
  capHeight = MAX(capHeight, _width);
  if (capHeight != _capHeight) {
    _capHeight = capHeight;
    [self update];
  }
}

- (void)setWidth:(double)width {
  if (width != _width) {
    _width = width;
    [self update];
  }
}

- (double)strokeWidthInEM {
  if (!_strokeWidthInEM) {
    _strokeWidthInEM = takram::math::clamp(
        std::round((_width * _fontInfo.cap_height) / _capHeight),
        kTKNTypefaceMinStrokeWidthInEM, kTKNTypefaceMaxStrokeWidthInEM);
  }
  return _strokeWidthInEM;
}

#pragma mark Typographic Properties

@dynamic familyName;
@dynamic proposedSize;
@dynamic unitsPerEM;
@dynamic ascender;
@dynamic descender;

- (NSString *)familyName {
  return [NSString stringWithUTF8String:_fontInfo.family_name.c_str()];
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
  const auto width = self.strokeWidthInEM;
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
