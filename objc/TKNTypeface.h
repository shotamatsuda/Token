//
//  TKNTypeface.h
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

#import <AppKit/AppKit.h>

#ifdef __cplusplus

#include "takram/graphics.h"
#include "token/glyph_outline.h"
#include "token/ufo.h"

#endif  // __cplusplus

#import "TKNTypefaceUnit.h"

@interface TKNTypeface : NSObject

- (nullable instancetype)initWithFileAtPath:(nullable NSString *)path
    NS_DESIGNATED_INITIALIZER;

#pragma mark Opening and Saving

@property (nonatomic, copy, readonly, nonnull) NSString *path;

- (BOOL)openFile:(nonnull NSString *)path;
- (BOOL)saveToFile:(nonnull NSString *)path;

#pragma mark Parameters

@property (nonatomic, assign) double capHeight;
@property (nonatomic, assign) double strokeWidth;
@property (nonatomic, assign) BOOL capHeightEqualsUnitsPerEM;
@property (nonatomic, assign) TKNTypefaceUnit capHeightUnit;
@property (nonatomic, assign) TKNTypefaceUnit strokeWidthUnit;

#pragma mark Typographic Properties

@property (nonatomic, strong, readonly, nonnull) NSString *familyName;
@property (nonatomic, strong, readonly, nonnull) NSString *styleName;
@property (nonatomic, assign, readonly) NSUInteger unitsPerEM;
@property (nonatomic, assign, readonly) NSInteger ascender;
@property (nonatomic, assign, readonly) NSInteger descender;
@property (nonatomic, assign, readonly) NSInteger lineGap;
@property (nonatomic, strong, readonly, nonnull) NSString *postscriptName;

#pragma mark Glyphs

- (nullable NSBezierPath *)glyphOutlineForName:(nonnull NSString *)name;
- (double)advanceOfGlyphForName:(nonnull NSString *)name;

#ifdef __cplusplus

- (nonnull NSBezierPath *)bezierPathWithShape:(const takram::Shape2d&)shape;
- (takram::Shape2d)strokeGlyph:(const token::ufo::Glyph&)glyph
                       outline:(const token::GlyphOutline&)outline;

#endif  // __cplusplus

@end
