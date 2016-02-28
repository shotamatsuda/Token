////
////  TKNTypefaceCore.m
////
////  The MIT License
////
////  Copyright (C) 2015-2016 Shota Matsuda
////
////  Permission is hereby granted, free of charge, to any person obtaining a
////  copy of this software and associated documentation files (the "Software"),
////  to deal in the Software without restriction, including without limitation
////  the rights to use, copy, modify, merge, publish, distribute, sublicense,
////  and/or sell copies of the Software, and to permit persons to whom the
////  Software is furnished to do so, subject to the following conditions:
////
////  The above copyright notice and this permission notice shall be included in
////  all copies or substantial portions of the Software.
////
////  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
////  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
////  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
////  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
////  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
////  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
////  DEALINGS IN THE SOFTWARE.
////
//
//#import "TKNTypefaceCore.h"
//
//#include <cassert>
//#include <cmath>
//#include <iterator>
//#include <string>
//#include <unordered_map>
//#include <utility>
//
//#include <boost/lexical_cast.hpp>
//#include <boost/filesystem.hpp>
//#include <boost/format.hpp>
//
//#include "takram/graphics.h"
//#include "takram/math.h"
//#include "token/afdko.h"
//#include "token/glyph_outline.h"
//#include "token/glyph_stroker.h"
//#include "token/ufo.h"
//
//@interface TKNTypefaceCore () {
// @private
//  token::ufo::FontInfo _fontInfo;
//  token::ufo::Glyphs _glyphs;
//  std::unordered_map<std::string, token::GlyphOutline> _glyphOutlines;
//  std::unordered_map<std::string, takram::Shape2d> _glyphShapes;
//}
//
//@end
//
//@implementation TKNTypefaceCore
//
//#pragma mark Typographic Properties
//
//@dynamic familyName;
//@dynamic unitsPerEM;
//@dynamic ascender;
//@dynamic descender;
//@dynamic lineGap;
//
//- (NSString *)familyName {
//  return [NSString stringWithUTF8String:_fontInfo.family_name.c_str()];
//}
//
//+ (NSSet *)keyPathsForValuesAffectingFamilyName {
//  return [NSSet setWithObjects:@"metricsType", nil];
//}
//
//- (NSUInteger)unitsPerEM {
//  return _fontInfo.units_per_em;
//}
//
//+ (NSSet *)keyPathsForValuesAffectingUnitsPerEM {
//  return [NSSet setWithObjects:@"metricsType", nil];
//}
//
//- (double)scale {
//  if (!_fontInfo.units_per_em) {
//    return 0.0;
//  }
//  return (_fontInfo.ascender - _fontInfo.descender) / _fontInfo.units_per_em;
//}
//
//+ (NSSet *)keyPathsForValuesAffectingScale {
//  return [NSSet setWithObjects:@"metricsType", nil];
//}
//
//- (NSInteger)ascender {
//  return _fontInfo.ascender;
//}
//
//+ (NSSet *)keyPathsForValuesAffectingAscender {
//  return [NSSet setWithObjects:@"metricsType", nil];
//}
//
//- (NSInteger)descender {
//  return _fontInfo.descender;
//}
//
//+ (NSSet *)keyPathsForValuesAffectingDescender {
//  return [NSSet setWithObjects:@"metricsType", nil];
//}
//
//- (NSInteger)lineGap {
//  return _fontInfo.open_type_hhea_line_gap;
//}
//
//+ (NSSet *)keyPathsForValuesAffectingLineGap {
//  return [NSSet setWithObjects:@"metricsType", nil];
//}
//
//@end
