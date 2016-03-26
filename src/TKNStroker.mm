//
//  TKNStroker.m
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

#define TAKRAM_HAS_COREGRAPHICS 1

#import "TKNStroker.h"

#include <cassert>
#include <iterator>
#include <string>
#include <unordered_map>
#include <utility>

#include <boost/filesystem.hpp>

#include "takram/graphics.h"
#include "takram/math.h"
#include "token/afdko.h"
#include "token/glyph_outline.h"
#include "token/glyph_stroker.h"
#include "token/ufo.h"

@interface TKNStroker () {
 @private
  token::ufo::FontInfo _fontInfo;
  token::ufo::Glyphs _glyphs;
  std::unordered_map<std::string, token::GlyphOutline> _glyphOutlines;
  std::unordered_map<std::string, takram::Shape2d> _glyphShapes;
  std::unordered_map<std::string, takram::Rect2d> _glyphBounds;
  std::unordered_map<std::string, token::ufo::glif::Advance> _glyphAdvances;
  NSMutableDictionary *_glyphBezierPaths;
}

#pragma mark Glyphs

- (BOOL)strokeGlyph:(const token::ufo::Glyph&)glyph;
- (NSBezierPath *)bezierPathWithShape:(const takram::Shape2d&)shape;

#pragma mark Exporting

- (BOOL)saveFontInfoAtPath:(const std::string&)path;
- (BOOL)saveGlyphsAtPath:(const std::string&)path;

@end

@implementation TKNStroker

- (instancetype)initWithContentsOfURL:(nonnull NSURL *)URL {
  self = [super init];
  if (self) {
    _fontInfo = token::ufo::FontInfo(URL.path.UTF8String);
    _glyphs = token::ufo::Glyphs(URL.path.UTF8String);
    _glyphBezierPaths = [NSMutableDictionary dictionary];
    _styleName = [NSString stringWithUTF8String:
        _fontInfo.style_name.c_str()];
    _postscriptName = [NSString stringWithUTF8String:
        _fontInfo.postscript_font_name.c_str()];
    _fullName = [NSString stringWithUTF8String:
        _fontInfo.postscript_full_name.c_str()];
    _URL = URL;
    _strokePrecision = 250.0 / _fontInfo.units_per_em;
    _strokeShiftIncrement = 0.0001;
    _strokeShiftLimit = 0.1;
  }
  return self;
}

- (id)copyWithZone:(NSZone *)zone {
  TKNStroker *copy = [[[self class] allocWithZone:zone] init];
  copy->_fontInfo = _fontInfo;
  copy->_glyphs = _glyphs;
  copy->_glyphOutlines = _glyphOutlines;
  copy->_glyphShapes = _glyphShapes;
  copy->_glyphBounds = _glyphBounds;
  copy->_glyphAdvances = _glyphAdvances;
  copy->_glyphBezierPaths = [_glyphBezierPaths copy];
  copy->_URL = [_URL copy];
  copy->_strokeWidth = _strokeWidth;
  copy->_strokePrecision = _strokePrecision;
  copy->_strokeShiftIncrement = _strokeShiftIncrement;
  copy->_strokeShiftLimit = _strokeShiftLimit;
  copy.styleName = self.styleName;
  copy.fullName = self.fullName;
  copy.postscriptName = self.postscriptName;
  copy.UPEM = self.UPEM;
  return copy;
}

#pragma mark Stroke Width

- (void)setStrokeWidth:(double)strokeWidth {
  strokeWidth = std::round(strokeWidth);
  if (strokeWidth != _strokeWidth) {
    _strokeWidth = strokeWidth;
    _glyphOutlines.clear();
    _glyphShapes.clear();
    _glyphBounds.clear();
    _glyphAdvances.clear();
    [_glyphBezierPaths removeAllObjects];
  }
}

#pragma mark Properties

@dynamic familyName;
@dynamic UPEM;
@dynamic ascender;
@dynamic descender;
@dynamic capHeight;
@dynamic lineGap;

- (NSString *)familyName {
  return [NSString stringWithUTF8String:_fontInfo.family_name.c_str()];
}

- (double)UPEM {
  return _fontInfo.units_per_em;
}

- (void)setUPEM:(double)UPEM {
  _fontInfo.units_per_em = UPEM;
}

- (double)ascender {
  return _fontInfo.ascender;
}

- (double)descender {
  return _fontInfo.descender;
}

- (double)capHeight {
  return _fontInfo.cap_height;
}

- (double)lineGap {
  return _fontInfo.open_type_hhea_line_gap;
}

#pragma mark Glyphs

- (NSBezierPath *)glyphBezierPathForName:(NSString *)name {
  [self strokeGlyphForName:name];
  const auto shape = _glyphShapes.find(name.UTF8String);
  assert(shape != std::end(_glyphShapes));
  auto bezierPath = [self bezierPathWithShape:shape->second];
  _glyphBezierPaths[name] = bezierPath;
  return bezierPath;
}

- (double)glyphAdvanceForName:(NSString *)name {
  [self strokeGlyphForName:name];
  const auto advance = _glyphAdvances.find(name.UTF8String);
  assert(advance != std::end(_glyphAdvances));
  return advance->second.width;
}

- (CGRect)glyphBoundsForName:(NSString *)name {
  [self strokeGlyphForName:name];
  const auto bounds = _glyphBounds.find(name.UTF8String);
  assert(bounds != std::end(_glyphBounds));
  return bounds->second;
}

- (BOOL)strokeGlyphForName:(nonnull NSString *)name {
  auto glyph = _glyphs.find(name.UTF8String);
  assert(glyph);
  return [self strokeGlyph:*glyph];
}

- (BOOL)strokeGlyph:(const token::ufo::Glyph&)glyph {
  const auto found = _glyphOutlines.find(glyph.name);
  if (found != std::end(_glyphOutlines)) {
    return false;
  }
  const auto& outline = _glyphOutlines.emplace(
      glyph.name,
      token::GlyphOutline(glyph)).first->second;
  token::GlyphStroker stroker;
  stroker.set_width(_strokeWidth);
  stroker.set_precision(_strokePrecision);
  stroker.set_shift_increment(_strokeShiftIncrement);
  stroker.set_shift_limit(_strokeShiftLimit);
  auto pair = stroker(_fontInfo, glyph, outline);
  _glyphShapes.emplace(glyph.name, pair.first);
  _glyphBounds.emplace(glyph.name, pair.first.bounds(true));
  _glyphAdvances.emplace(glyph.name, pair.second);
  return true;
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

#pragma mark Saving

- (BOOL)saveToURL:(NSURL *)URL error:(NSError **)error {
  NSFileManager *fileManager = [NSFileManager defaultManager];
  if (![URL.URLByDeletingLastPathComponent
          checkResourceIsReachableAndReturnError:nil]) {
    if (![fileManager createDirectoryAtURL:URL.URLByDeletingLastPathComponent
               withIntermediateDirectories:YES
                                attributes:nil
                                     error:error]) {
      return NO;
    }
  }
  if (![fileManager copyItemAtURL:_URL toURL:URL error:error] ||
      ![self saveFontInfoAtPath:URL.path.UTF8String] ||
      ![self saveGlyphsAtPath:URL.path.UTF8String]) {
    return NO;
  }
  return YES;
}

- (BOOL)saveFontInfoAtPath:(const std::string&)path {
  // TODO(shotamatsuda): Adjust x-height
  // TODO(shotamatsuda): Change style_map_style_name
  token::ufo::FontInfo fontInfo = _fontInfo;
  fontInfo.style_name = _styleName.UTF8String;
  fontInfo.macintosh_fond_name = _fullName.UTF8String;
  fontInfo.postscript_font_name = _postscriptName.UTF8String;
  fontInfo.postscript_full_name = _fullName.UTF8String;
  fontInfo.open_type_name_compatible_full_name = _fullName.UTF8String;
  fontInfo.open_type_name_preferred_subfamily_name = _styleName.UTF8String;
  fontInfo.open_type_name_unique_id =
      fontInfo.open_type_name_version + ";" +
      fontInfo.open_type_os2_vendor_id + ";" +
      fontInfo.postscript_font_name;
  fontInfo.open_type_os2_strikeout_size = _strokeWidth;
  fontInfo.open_type_os2_strikeout_position =
      std::round((fontInfo.x_height + _strokeWidth) / 2.0);
  fontInfo.postscript_stem_snap_h.clear();
  fontInfo.postscript_stem_snap_h.emplace_back(_strokeWidth);
  fontInfo.postscript_stem_snap_v.clear();
  fontInfo.postscript_stem_snap_v.emplace_back(_strokeWidth);
  fontInfo.postscript_underline_thickness = _strokeWidth;
  fontInfo.postscript_underline_position = -std::round(_strokeWidth * 2.5);
  return fontInfo.save(path);
}

- (BOOL)saveGlyphsAtPath:(const std::string&)path {
  const auto glyphsPath = boost::filesystem::path(path) / "glyphs";
  for (auto glyph : _glyphs) {
    [self strokeGlyph:glyph];
    assert(_glyphOutlines.find(glyph.name) != std::end(_glyphOutlines));
    assert(_glyphShapes.find(glyph.name) != std::end(_glyphShapes));
    assert(_glyphAdvances.find(glyph.name) != std::end(_glyphAdvances));
    auto outline = _glyphOutlines.at(glyph.name);
    outline.shape() = _glyphShapes.at(glyph.name);
    glyph.advance = _glyphAdvances.at(glyph.name);
    const auto glyphPath = glyphsPath / _glyphs.filename(glyph.name);
    if (!outline.glyph(glyph).save(glyphPath.string())) {
      return NO;
    }
  }
  return YES;
}

@end
