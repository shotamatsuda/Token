// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#import <AppKit/AppKit.h>

@interface TKNStroker : NSObject <NSCopying>

// MARK: Opening and Saving

- (nullable instancetype)initWithContentsOfURL:(nonnull NSURL *)url;

@property (nonatomic, copy, readonly, nonnull) NSURL *url;

// MARK: Stroking

@property (nonatomic, assign) double strokeWidth;
@property (nonatomic, assign) double strokePrecision;
@property (nonatomic, assign) double strokeShiftIncrement;
@property (nonatomic, assign) double strokeShiftLimit;

// MARK: Properties

@property (nonatomic, copy, readonly, nonnull) NSString *familyName;
@property (nonatomic, copy, nonnull) NSString *styleName;
@property (nonatomic, copy, nonnull) NSString *fullName;
@property (nonatomic, copy, nonnull) NSString *postscriptName;
@property (nonatomic, assign) double UPEM;
@property (nonatomic, assign, readonly) double ascender;
@property (nonatomic, assign, readonly) double descender;
@property (nonatomic, assign, readonly) double capHeight;
@property (nonatomic, assign, readonly) double lineGap;

// MARK: Glyphs

- (BOOL)strokeGlyphForName:(nonnull NSString *)name;
- (nullable NSBezierPath *)glyphBezierPathForName:(nonnull NSString *)name;
- (double)glyphAdvanceForName:(nonnull NSString *)name;
- (CGRect)glyphBoundsForName:(nonnull NSString *)name;

// MARK: Saving

- (BOOL)saveToURL:(nonnull NSURL *)url
            error:(NSError * _Nullable * _Nullable)error;

@end
