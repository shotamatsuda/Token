//
//  TKNStroker.h
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

#import <AppKit/AppKit.h>

@interface TKNStroker : NSObject <NSCopying>

#pragma mark Opening and Saving

- (nullable instancetype)initWithContentsOfURL:(nonnull NSURL *)URL;

@property (nonatomic, copy, readonly, nonnull) NSURL *URL;

#pragma mark Stroking

@property (nonatomic, assign) double strokeWidth;
@property (nonatomic, assign) double strokePrecision;
@property (nonatomic, assign) double strokeShiftIncrement;
@property (nonatomic, assign) double strokeShiftLimit;

#pragma mark Properties

@property (nonatomic, copy, readonly, nonnull) NSString *familyName;
@property (nonatomic, copy, nonnull) NSString *styleName;
@property (nonatomic, copy, nonnull) NSString *fullName;
@property (nonatomic, copy, nonnull) NSString *postscriptName;
@property (nonatomic, assign) double UPEM;
@property (nonatomic, assign, readonly) double ascender;
@property (nonatomic, assign, readonly) double descender;
@property (nonatomic, assign, readonly) double capHeight;
@property (nonatomic, assign, readonly) double lineGap;

#pragma mark Glyphs

- (BOOL)strokeGlyphForName:(nonnull NSString *)name;
- (nullable NSBezierPath *)glyphBezierPathForName:(nonnull NSString *)name;
- (double)glyphAdvanceForName:(nonnull NSString *)name;
- (CGRect)glyphBoundsForName:(nonnull NSString *)name;

#pragma mark Saving

- (BOOL)saveToURL:(nonnull NSURL *)URL
            error:(NSError * _Nullable * _Nullable)error;

@end
