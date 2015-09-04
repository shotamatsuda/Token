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

@interface TKNTypeface () {
 @private
  token::ufo::FontInfo _fontInfo;
  token::ufo::Glyphs _glyphs;
  std::unordered_map<std::string, token::GlyphOutline> _glyphOutlines;
  std::unordered_map<std::string, takram::Shape2d> _glyphShapes;
}

@property (nonatomic, strong) NSMutableDictionary *glyphBezierPaths;

@end

@implementation TKNTypeface

- (instancetype)init {
  return [self initWithFileAtPath:nil];
}

- (instancetype)initWithFileAtPath:(NSString *)path {
  self = [super init];
  if (self) {
    _glyphBezierPaths = [NSMutableDictionary dictionary];
    _width = 50.0;
    if (path) {
      [self openFile:path];
    }
  }
  return self;
}

- (void)openFile:(NSString *)path {
  _path = path;
  _fontInfo.open(_path.UTF8String);
  _glyphs.open(_path.UTF8String);
  for (auto& glyph : _glyphs) {
    _glyphOutlines.emplace(glyph.name, token::GlyphOutline(glyph));
  }
}

- (void)saveToFile:(NSString *)path {
  NSFileManager *manager = [NSFileManager defaultManager];
  [manager removeItemAtPath:path error:NULL];
  [manager copyItemAtPath:_path toPath:path error:NULL];

  // Glyphs
  const auto glyphsPath = boost::filesystem::path(path.UTF8String) / "glyphs";
  for (const auto& glyph : _glyphs) {
    assert(_glyphOutlines.find(glyph.name) != std::end(_glyphOutlines));
    auto outline = _glyphOutlines.at(glyph.name);
    outline.shape() = [self strokeGlyph:glyph outline:outline];
    const auto glyphPath = glyphsPath / _glyphs.filename(glyph.name);
    outline.glyph(glyph).save(glyphPath.string());
  }
}

#pragma mark Parameters

- (void)setWidth:(double)width {
  if (width != _width) {
    _width = width;
    _glyphShapes.clear();
    [_glyphBezierPaths removeAllObjects];
  }
}

#pragma mark Glyphs

- (NSUInteger)unitsPerEM {
  return _fontInfo.units_per_em;
}

- (NSInteger)ascender {
  return _fontInfo.ascender;
}

- (NSInteger)descender {
  return _fontInfo.descender;
}

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
  _glyphShapes.emplace(name.UTF8String, std::move(shape));
  _glyphBezierPaths[name] = [self bezierPathWithShape:shape];
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
  const auto advance = glyph.advance->width;
  const auto ascender = _fontInfo.ascender;
  const auto scale = (ascender - _width) / ascender;
  const takram::Vec2d center(advance / 2.0, ascender / 2.0);
  auto stroked = outline;
  for (auto& command : stroked.shape()) {
    command.point() = center + (command.point() - center) * scale;
    command.control1() = center + (command.control1() - center) * scale;
    command.control2() = center + (command.control2() - center) * scale;
  }

  // Stroking and path simplification
  token::GlyphStroker stroker;
  stroker.set_width(_width);
  takram::Shape2d shape = stroker.stroke(stroked);
  shape = stroker.simplify(shape);
  shape.convertConicsToQuadratics();
  shape.convertQuadraticsToCubics();
  shape.removeDuplicates(1.0);
  return std::move(shape);
}

@end
