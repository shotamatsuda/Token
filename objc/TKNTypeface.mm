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
#include <boost/filesystem.hpp>
#include <boost/format.hpp>

#include "takram/graphics.h"
#include "takram/math.h"
#include "token/afdko.h"
#include "token/glyph_outline.h"
#include "token/glyph_stroker.h"
#include "token/ufo.h"

#import "TKNFilePaths.h"
#import "TKNMainWindowController.h"
#import "TKNTypefaceUnit.h"

static const double kTKNTypefaceStrokingRetryShift = 0.0001;
static const double kTKNTypefaceStrokingRetryShiftLimit = 0.1;
static const double kTKNTypefaceMinStrokeWidth = 10.0;
static const double kTKNTypefaceMaxStrokeWidth = 130.0;

@interface TKNTypeface () {
 @private
  token::ufo::FontInfo _fontInfo;
  token::ufo::Glyphs _glyphs;
  std::unordered_map<std::string, token::GlyphOutline> _glyphOutlines;
  std::unordered_map<std::string, takram::Shape2d> _glyphShapes;
  double _strokeWidth;
}

@property (nonatomic, strong) NSMutableDictionary *glyphBezierPaths;

- (void)parameterDidChange;
- (void)physicalParameterDidChange;
- (void)typographicParameterDidChange;

#pragma mark Opening and Saving

@property (nonatomic, copy, nonnull) NSString *path;

- (NSString *)createUnifiedFontObject:(NSString *)directory;
- (void)updateFontInfoInUnifiedFontObject:(NSString *)path;
- (void)updateGlyphsInUnifiedFontObject:(NSString *)path;
- (NSString *)createOpenTypeWithUnifiedFontObject:(NSString *)path;

#pragma mark Typographic Properties

@property (nonatomic, strong) NSString *styleName;
@property (nonatomic, strong) NSString *postscriptName;

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
    _metricsType = kTKNTypefaceMetricsTypePhysical;
    _physicalStrokeWidth = 0.2;
    _physicalCapHeight = 2.5;
    _typographicStrokeWidth = 50;
    [self openFile:path];
    [self parameterDidChange];
  }
  return self;
}

- (double)strokeWidth {
  if (!_strokeWidth) {
    switch (_metricsType) {
      case kTKNTypefaceMetricsTypePhysical: {
        const double strokeWidth = TKNTypefaceUnitConvert(
            _physicalStrokeWidth, _strokeWidthUnit, kTKNTypefaceUnitPoint);
        const double capHeight = TKNTypefaceUnitConvert(
            _physicalCapHeight, _capHeightUnit, kTKNTypefaceUnitPoint);
        _strokeWidth = takram::math::clamp(
            std::round((strokeWidth * _fontInfo.cap_height) / capHeight),
            kTKNTypefaceMinStrokeWidth, kTKNTypefaceMaxStrokeWidth);
        break;
      }
      case kTKNTypefaceMetricsTypeTypographic:
        _strokeWidth = _typographicStrokeWidth;
        break;
      default:
        assert(false);
        break;
    }
  }
  return _strokeWidth;
}

- (void)parameterDidChange {
  switch (_metricsType) {
    case kTKNTypefaceMetricsTypePhysical:
      [self physicalParameterDidChange];
      break;
    case kTKNTypefaceMetricsTypeTypographic:
      [self typographicParameterDidChange];
      break;
    default:
      assert(false);
      break;
  }
  // Invalidate existing glyph shapes and bezier paths
  _strokeWidth = 0.0;
  _glyphShapes.clear();
  [_glyphBezierPaths removeAllObjects];
}

- (void)physicalParameterDidChange {
  // Update style names
  NSNumberFormatter *formatter = [[NSNumberFormatter alloc] init];
  formatter.numberStyle = NSNumberFormatterDecimalStyle;
  formatter.minimumFractionDigits = 2;
  formatter.maximumFractionDigits = 2;
  std::string style;
  style += [formatter stringFromNumber:
      [NSNumber numberWithDouble:_physicalStrokeWidth]].UTF8String;
  style += TKNTypefaceUnitGetShortName(_strokeWidthUnit).UTF8String;
  style += " / ";
  style += [formatter stringFromNumber:
      [NSNumber numberWithDouble:_physicalCapHeight]].UTF8String;
  style += TKNTypefaceUnitGetShortName(_capHeightUnit).UTF8String;
  std::string postscriptStyle;
  postscriptStyle += [formatter stringFromNumber:
      [NSNumber numberWithDouble:_physicalStrokeWidth]].UTF8String;
  postscriptStyle += TKNTypefaceUnitGetShortName(_strokeWidthUnit).UTF8String;
  postscriptStyle += "-";
  postscriptStyle += [formatter stringFromNumber:
      [NSNumber numberWithDouble:_physicalCapHeight]].UTF8String;
  postscriptStyle += TKNTypefaceUnitGetShortName(_capHeightUnit).UTF8String;
  self.styleName = [NSString stringWithUTF8String:style.c_str()];
  self.postscriptName = [[self.familyName stringByAppendingString:@"-"]
      stringByAppendingString:
          [NSString stringWithUTF8String:postscriptStyle.c_str()]];
}

- (void)typographicParameterDidChange {
  // Update style names
  std::string style = std::to_string(_typographicStrokeWidth);
  self.styleName = [NSString stringWithUTF8String:style.c_str()];
  self.postscriptName = [[self.familyName stringByAppendingString:@"-"]
      stringByAppendingString:[NSString stringWithUTF8String:style.c_str()]];
}

#pragma mark Opening and Saving

- (BOOL)openFile:(NSString *)path {
  NSString *fileName = nil;
  switch (_metricsType) {
    case kTKNTypefaceMetricsTypePhysical:
      fileName = @"scaled.ufo";
      break;
    case kTKNTypefaceMetricsTypeTypographic:
      fileName = @"default.ufo";
      break;
    default:
      assert(false);
      break;
  }
  path = [path stringByAppendingPathComponent:fileName];
  _fontInfo = token::ufo::FontInfo(path.UTF8String);
  _glyphs = token::ufo::Glyphs(path.UTF8String);
  _glyphOutlines.clear();
  for (auto& glyph : _glyphs) {
    _glyphOutlines.emplace(glyph.name, token::GlyphOutline(glyph));
  }
  [self parameterDidChange];
  self.path = path;
  return YES;
}

- (BOOL)saveToFile:(NSString *)path {
  NSString *uniqueString = [NSProcessInfo processInfo].globallyUniqueString;
  NSString *workingDirectoryPath = [NSTemporaryDirectory()
      stringByAppendingPathComponent:uniqueString];
  NSString *ufoPath = [self createUnifiedFontObject:workingDirectoryPath];
  [self updateFontInfoInUnifiedFontObject:ufoPath];
  [self updateGlyphsInUnifiedFontObject:ufoPath];
  NSString *otfPath = [self createOpenTypeWithUnifiedFontObject:ufoPath];
  if (!otfPath) {
    return NO;
  }
  NSFileManager *fileManager = [NSFileManager defaultManager];
  NSError *error = nil;
  if ([fileManager fileExistsAtPath:path]) {
    if (![fileManager removeItemAtPath:path error:&error]) {
      [[NSAlert alertWithError:error]
          beginSheetModalForWindow:[NSApp mainWindow]
          completionHandler:nil];
      return NO;
    }
  }
  NSString *directory = path.stringByDeletingLastPathComponent;
  if (![fileManager fileExistsAtPath:directory]) {
    if (![fileManager createDirectoryAtPath:directory
                withIntermediateDirectories:YES
                                 attributes:nil
                                      error:&error]) {
      [[NSAlert alertWithError:error]
          beginSheetModalForWindow:[NSApp mainWindow]
          completionHandler:nil];
      return NO;
    }
  }
  if (![fileManager copyItemAtPath:otfPath toPath:path error:&error]) {
    [[NSAlert alertWithError:error]
        beginSheetModalForWindow:[NSApp mainWindow]
        completionHandler:nil];
    return NO;
  }
  if (![fileManager removeItemAtPath:workingDirectoryPath error:&error]) {
    [[NSAlert alertWithError:error]
        beginSheetModalForWindow:[NSApp mainWindow]
        completionHandler:nil];
    return NO;
  }
  return YES;
}

- (NSString *)createUnifiedFontObject:(NSString *)directory {
  NSString *ufoPath = [directory stringByAppendingPathComponent:
      [_path.lastPathComponent.stringByDeletingPathExtension
          stringByAppendingPathExtension:@"ufo"]];
  NSFileManager *fileManager = [NSFileManager defaultManager];
  NSError *error = nil;
  if (![fileManager createDirectoryAtPath:directory
              withIntermediateDirectories:YES
                               attributes:nil
                                    error:&error]) {
    [[NSAlert alertWithError:error]
        beginSheetModalForWindow:[NSApp mainWindow]
        completionHandler:nil];
    return nil;
  }
  if (![fileManager copyItemAtPath:_path toPath:ufoPath error:&error]) {
    [[NSAlert alertWithError:error]
        beginSheetModalForWindow:[NSApp mainWindow]
        completionHandler:nil];
    return nil;
  }
  return ufoPath;
}

- (void)updateFontInfoInUnifiedFontObject:(NSString *)path {
  token::ufo::FontInfo fontInfo = _fontInfo;
  NSNumberFormatter *formatter = [[NSNumberFormatter alloc] init];
  formatter.numberStyle = NSNumberFormatterDecimalStyle;
  formatter.minimumFractionDigits = 2;
  formatter.maximumFractionDigits = 2;
  const std::string style = _styleName.UTF8String;
  const std::string postscriptName = _postscriptName.UTF8String;
  const std::string fullName = fontInfo.family_name + " " + style;
  // TODO(shotamatsuda): Adjust x-height
  fontInfo.style_name = style;
  fontInfo.style_map_style_name = style;
  fontInfo.macintosh_fond_name = fullName;
  fontInfo.postscript_font_name = postscriptName;
  fontInfo.postscript_full_name = fullName;
  fontInfo.open_type_name_compatible_full_name = fullName;
  fontInfo.open_type_name_preferred_subfamily_name = style;
  fontInfo.open_type_name_unique_id =
      fontInfo.open_type_name_version + ";" +
      fontInfo.open_type_os2_vendor_id + ";" +
      fontInfo.postscript_font_name;
  fontInfo.open_type_os2_strikeout_size = _strokeWidth;
  fontInfo.open_type_os2_strikeout_position =
      std::round((fontInfo.x_height + _strokeWidth) / 2.0);
  fontInfo.postscript_stem_snap_h.clear();
  fontInfo.postscript_stem_snap_h.emplace_back(self.strokeWidth);
  fontInfo.postscript_stem_snap_v.clear();
  fontInfo.postscript_stem_snap_v.emplace_back(self.strokeWidth);
  fontInfo.postscript_underline_thickness = _strokeWidth;
  fontInfo.postscript_underline_position = -std::round(_strokeWidth * 2.5);
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
  NSString *directory = path.stringByDeletingLastPathComponent;
  NSString *otfPath = [directory stringByAppendingPathComponent:
      [_path.lastPathComponent.stringByDeletingPathExtension
          stringByAppendingPathExtension:@"otf"]];
  const auto fdkPath = boost::filesystem::path(TKNAdobeFDKPath().UTF8String);
  const auto toolsPath = fdkPath / "Tools" / "osx";
  if (!boost::filesystem::exists(toolsPath)) {
    [NSApp sendAction:@selector(installAdobeFDK:) to:nil from:self];
    return nil;
  }
  token::afdko::checkOutlines(toolsPath.string(), path.UTF8String);
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

#pragma mark Parameters

- (void)setMetricsType:(TKNTypefaceMetricsType)metricsType {
  if (metricsType != _metricsType) {
    _metricsType = metricsType;
    [self openFile:_path.stringByDeletingLastPathComponent];
    [self parameterDidChange];
  }
}

- (void)setPhysicalStrokeWidth:(double)strokeWidth {
  const double capHeight = TKNTypefaceUnitConvert(
      _physicalCapHeight, _capHeightUnit, _strokeWidthUnit);
  const double coeff = capHeight / _fontInfo.cap_height;
  double min = coeff * kTKNTypefaceMinStrokeWidth;
  double max = coeff * kTKNTypefaceMaxStrokeWidth;
  min = std::ceil(min * 100.0) / 100.0;
  max = std::floor(max * 100.0) / 100.0;
  strokeWidth = takram::math::clamp(strokeWidth, min, max);
  if (strokeWidth != _physicalStrokeWidth) {
    _physicalStrokeWidth = strokeWidth;
    [self parameterDidChange];
  }
}

- (void)setPhysicalCapHeight:(double)capHeight {
  const double strokeWidth = TKNTypefaceUnitConvert(
      _physicalStrokeWidth, _strokeWidthUnit, _capHeightUnit);
  const double numerator = _fontInfo.cap_height * strokeWidth;
  double min = numerator / kTKNTypefaceMaxStrokeWidth;
  double max = numerator / kTKNTypefaceMinStrokeWidth;
  min = std::ceil(min * 100.0) / 100.0;
  max = std::floor(max * 100.0) / 100.0;
  capHeight = takram::math::clamp(capHeight, min, max);
  if (capHeight != _physicalCapHeight) {
    _physicalCapHeight = capHeight;
    [self parameterDidChange];
  }
}

- (void)setStrokeWidthUnit:(TKNTypefaceUnit)strokeWidthUnit {
  if (strokeWidthUnit != _strokeWidthUnit) {
    TKNTypefaceUnit oldValue = _strokeWidthUnit;
    _strokeWidthUnit = strokeWidthUnit;
    self.physicalStrokeWidth = TKNTypefaceUnitConvert(
        _physicalStrokeWidth, oldValue, _strokeWidthUnit);
  }
}

- (void)setCapHeightUnit:(TKNTypefaceUnit)capHeightUnit {
  if (capHeightUnit != _capHeightUnit) {
    TKNTypefaceUnit oldValue = _capHeightUnit;
    _capHeightUnit = capHeightUnit;
    self.physicalCapHeight = TKNTypefaceUnitConvert(
        _physicalCapHeight, oldValue, _capHeightUnit);
  }
}

- (void)setTypographicStrokeWidth:(NSInteger)strokeWidth {
  strokeWidth = takram::math::clamp(
      std::round(strokeWidth),
      kTKNTypefaceMinStrokeWidth,
      kTKNTypefaceMaxStrokeWidth);
  if (strokeWidth != _typographicStrokeWidth) {
    _typographicStrokeWidth = strokeWidth;
    [self parameterDidChange];
  }
}

- (NSInteger)minTypographicStrokeWidth {
  return kTKNTypefaceMinStrokeWidth;
}

- (NSInteger)midTypographicStrokeWidth {
  return kTKNTypefaceMinStrokeWidth +
      (kTKNTypefaceMaxStrokeWidth - kTKNTypefaceMinStrokeWidth) / 2.0;
}

- (NSInteger)maxTypographicStrokeWidth {
  return kTKNTypefaceMaxStrokeWidth;
}

#pragma mark Typographic Properties

@dynamic familyName;
@dynamic unitsPerEM;
@dynamic ascender;
@dynamic descender;
@dynamic lineGap;

- (NSString *)familyName {
  return [NSString stringWithUTF8String:_fontInfo.family_name.c_str()];
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

- (NSInteger)lineGap {
  return _fontInfo.open_type_hhea_line_gap;
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

- (CGRect)boundingRectOfGlyphForName:(NSString *)name {
  const auto glyph = _glyphs.find(name.UTF8String);
  if (!glyph) {
    return CGRectZero;
  }
  [self glyphOutlineForName:name];
  return _glyphShapes.at(name.UTF8String).bounds(true);
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
  const auto strokeWidth = self.strokeWidth;
  const auto typoCapHeight = _fontInfo.cap_height;
  const auto scale = (typoCapHeight - strokeWidth) / typoCapHeight;
  const auto bounds = outline.shape().bounds(true);
  const takram::Vec2d center(bounds.midX(), typoCapHeight / 2.0);
  auto scaledOutline = outline;
  for (auto& command : scaledOutline.shape()) {
    command.point() = center + (command.point() - center) * scale;
    command.control1() = center + (command.control1() - center) * scale;
    command.control2() = center + (command.control2() - center) * scale;
  }

  // Stroking and path simplification
  token::GlyphStroker stroker;
  stroker.set_width(strokeWidth);
  takram::Shape2d shape;
  // Check for the number of contours of the resulting shape and retry if that
  // differs from the expected value, because the path simplification
  // occationally fails.
  bool success{};
  double shift{};
  bool negative{};
  for (; shift < kTKNTypefaceStrokingRetryShiftLimit;
       shift += kTKNTypefaceStrokingRetryShift) {
    for (negative = false; negative != true; negative = !negative) {
      stroker.set_width(strokeWidth + (negative ? -shift : shift));
      shape = stroker.stroke(scaledOutline);
      shape = stroker.simplify(shape);
      std::size_t numberOfContours{};
      std::size_t numberOfHoles{};
      for (const auto& path : shape.paths()) {
        if (path.direction() != takram::PathDirection::UNDEFINED) {
          ++numberOfContours;
        }
        if (path.direction() == takram::PathDirection::COUNTER_CLOCKWISE) {
          ++numberOfHoles;
        }
      }
      if (numberOfContours == glyph.lib->number_of_contours &&
          numberOfHoles == glyph.lib->number_of_holes) {
        success = true;
        break;
      }
    }
    if (success) {
      break;
    }
  }
  if (success) {
    if (shift) {
      NSLog(@"Stroking succeeded by shifting stroke width by %f.",
            (negative ? -shift : shift));
    }
    shape.convertConicsToQuadratics();
    shape.convertQuadraticsToCubics();
    shape.removeDuplicates(1.0);
  } else {
    NSLog(@"Stroking failed by shifting stroke width up to ±%f.",
          kTKNTypefaceStrokingRetryShiftLimit);
    shape.reset();
  }
  return std::move(shape);
}

@end
