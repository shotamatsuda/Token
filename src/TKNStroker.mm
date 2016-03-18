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
  NSMutableDictionary *_glyphBezierPaths;
}

#pragma mark Glyphs

- (NSBezierPath *)bezierPathWithShape:(const takram::Shape2d&)shape;
- (takram::Shape2d)strokeGlyph:(const token::ufo::Glyph&)glyph
                      outline:(const token::GlyphOutline&)outline;

#pragma mark Exporting

- (BOOL)updateFontInfoAtPath:(const std::string&)path;
- (BOOL)updateGlyphsAtPath:(const std::string&)path;

@end

@implementation TKNStroker

- (instancetype)initWithContentsOfURL:(nonnull NSURL *)URL {
  self = [super init];
  if (self) {
    _URL = URL;
    _strokeShiftIncrement = 0.0001;
    _strokeShiftLimit = 0.1;
    _fontInfo = token::ufo::FontInfo(URL.path.UTF8String);
    _glyphs = token::ufo::Glyphs(URL.path.UTF8String);
    for (auto& glyph : _glyphs) {
      _glyphOutlines.emplace(glyph.name, token::GlyphOutline(glyph));
    }
    _glyphBezierPaths = [NSMutableDictionary dictionary];
    _styleName = [NSString stringWithUTF8String:
        _fontInfo.style_name.c_str()];
    _postscriptName = [NSString stringWithUTF8String:
        _fontInfo.postscript_font_name.c_str()];
    _fullName = [NSString stringWithUTF8String:
        _fontInfo.postscript_full_name.c_str()];
  }
  return self;
}

#pragma mark Stroke Width

- (void)setStrokeWidth:(double)strokeWidth {
  if (strokeWidth != _strokeWidth) {
    _strokeWidth = strokeWidth;

    // Invalidate existing glyph shapes and bezier paths
    _glyphShapes.clear();
    [_glyphBezierPaths removeAllObjects];
  }
}

#pragma mark Properties

@dynamic familyName;
@dynamic ascender;
@dynamic descender;
@dynamic capHeight;
@dynamic lineGap;

- (NSString *)familyName {
  return [NSString stringWithUTF8String:_fontInfo.family_name.c_str()];
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

- (double)scale {
  return (_fontInfo.ascender - _fontInfo.descender) / _fontInfo.units_per_em;
}

#pragma mark Glyphs

- (NSBezierPath *)glyphBezierPathForName:(NSString *)name {
  const auto glyph = _glyphs.find(name.UTF8String);
  if (!glyph) {
    return nil;
  }
  NSBezierPath *path = _glyphBezierPaths[name];
  if (path) {
    return path;
  }
  if (!_strokeWidth) {
    return nil;
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

- (double)glyphAdvanceForName:(NSString *)name {
  const auto glyph = _glyphs.find(name.UTF8String);
  if (!glyph) {
    return double();
  }
  return glyph->advance->width;
}

- (CGRect)glyphBoundsForName:(NSString *)name {
  const auto glyph = _glyphs.find(name.UTF8String);
  if (!glyph) {
    return CGRectZero;
  }
  [self glyphBezierPathForName:name];
  const auto shape = _glyphShapes.find(name.UTF8String);
  if (shape == std::end(_glyphShapes)) {
    return CGRectZero;
  }
  return shape->second.bounds(true);
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
  // Scale the glyph outline from the center of the glyph.
  const auto strokeWidth = _strokeWidth;
  const auto capHeight = _fontInfo.cap_height;
  const auto scale = (capHeight - strokeWidth) / capHeight;
  const auto bounds = outline.shape().bounds(true);
  const takram::Vec2d center(bounds.midX(), capHeight / 2.0);
  auto scaledOutline = outline;
  for (auto& command : scaledOutline.shape()) {
    command.point() = center + (command.point() - center) * scale;
    command.control1() = center + (command.control1() - center) * scale;
    command.control2() = center + (command.control2() - center) * scale;
  }

  // Stroking
  token::GlyphStroker stroker;
  stroker.set_width(strokeWidth);
  stroker.set_shift_increment(_strokeShiftIncrement);
  stroker.set_shift_limit(_strokeShiftLimit);
  return stroker(glyph, scaledOutline);
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
  if (![fileManager copyItemAtURL:_URL toURL:URL error:error]) {
    return NO;
  }
  if (![self updateFontInfoAtPath:URL.path.UTF8String]) {
    return NO;
  }
  if (![self updateGlyphsAtPath:URL.path.UTF8String]) {
    return NO;
  }
  return YES;
}

- (BOOL)updateFontInfoAtPath:(const std::string&)path {
  // TODO(shotamatsuda): Adjust x-height
  token::ufo::FontInfo fontInfo = _fontInfo;
  fontInfo.style_name = _styleName.UTF8String;
  fontInfo.style_map_style_name = _styleName.UTF8String;
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

- (BOOL)updateGlyphsAtPath:(const std::string&)path {
  const auto glyphsPath = boost::filesystem::path(path) / "glyphs";
  for (const auto& glyph : _glyphs) {
    assert(_glyphOutlines.find(glyph.name) != std::end(_glyphOutlines));
    auto outline = _glyphOutlines.at(glyph.name);
    outline.shape() = [self strokeGlyph:glyph outline:outline];
    const auto glyphPath = glyphsPath / _glyphs.filename(glyph.name);
    if (!outline.glyph(glyph).save(glyphPath.string())) {
      return NO;
    }
  }
  return YES;
}

@end
