//
//  TKNTypefaceSampleView.m
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

#import "TKNTypefaceSampleView.h"

@interface TKNTypefaceSampleView ()

#pragma mark Drawing

@property (nonatomic, assign) double scale;
@property (nonatomic, strong) NSArray *lines;
@property (nonatomic, assign) CGAffineTransform transform;

- (void)drawLine:(NSArray *)glyphNames
        position:(CGPoint)position
       dirtyRect:(CGRect)dirtyRect;
- (void)drawGlyph:(NSString *)glyphName
         position:(CGPoint)position
        dirtyRect:(CGRect)dirtyRect;
- (void)drawSolidGlyph:(NSBezierPath *)outline;
- (void)drawOutlineGlyph:(NSBezierPath *)outline;

#pragma mark Resizing

- (void)resizeToFitLines;
- (CGSize)sizeForLines:(NSArray *)lines;

@end

@implementation TKNTypefaceSampleView

- (instancetype)initWithFrame:(NSRect)frame {
  self = [super initWithFrame:frame];
  if (self) {
    _scale = 0.085;
    _lines = @[
      @[@"A", @"B", @"C", @"D", @"E", @"F", @"G",
        @"H", @"I", @"J", @"K", @"L", @"M"],
      @[@"N", @"O", @"P", @"Q", @"R", @"S", @"T",
        @"U", @"V", @"W", @"X", @"Y", @"Z"],
      @[@"one", @"two", @"three", @"four", @"five",
        @"six", @"seven", @"eight", @"nine", @"zero"]
    ];
  }
  return self;
}

- (BOOL)acceptsFirstResponder {
  return YES;
}

#pragma mark Parameters

- (void)setInverted:(BOOL)inverted {
  if (inverted != _inverted) {
    _inverted = inverted;
    self.needsDisplay = YES;
  }
}

- (void)setOutlined:(BOOL)outlined {
  if (outlined != _outlined) {
    _outlined = outlined;
    self.needsDisplay = YES;
  }
}

#pragma mark Drawing

- (void)drawRect:(NSRect)dirtyRect {
  [self resizeToFitLines];

  // Remember this view's current transformation matrix because it's not
  // identity matrix. We'll use this later for intersection tests.
  _transform = CGContextGetCTM([NSGraphicsContext currentContext].CGContext);

  // Background color
  [NSGraphicsContext saveGraphicsState];
  NSColor *backgroundColor;
  if (_inverted) {
    backgroundColor = [NSColor blackColor];
  } else {
    backgroundColor = [NSColor whiteColor];
  }
  [backgroundColor setFill];
  NSRectFill(dirtyRect);

  // Move the origin to horizontal center and vertical top.
  NSAffineTransform *transform = [NSAffineTransform transform];
  CGSize size = self.frame.size;
  [transform translateXBy:size.width / 2.0 yBy:size.height];
  [transform scaleBy:_scale];
  [transform concat];

  // Draw every lines, moving down by ascender + descender per line.
  CGPoint position = CGPointZero;
  position.y -= _typeface.ascender - _typeface.descender;
  for (NSArray *line in _lines) {
    [self drawLine:line position:position dirtyRect:dirtyRect];
    position.x = 0.0;
    position.y -= _typeface.ascender - _typeface.descender;
  }
  [NSGraphicsContext restoreGraphicsState];
}

- (void)drawLine:(NSArray *)line
        position:(CGPoint)position
       dirtyRect:(CGRect)dirtyRect {
  // Derive the sum of advances in this line for centering.
  CGFloat lineWidth = 0.0;
  for (NSString *name in line) {
    lineWidth += [_typeface advanceOfGlyphForName:name];
  }
  [NSGraphicsContext saveGraphicsState];
  NSAffineTransform *transform = [NSAffineTransform transform];
  [transform translateXBy:-lineWidth / 2.0 yBy:0.0];
  [transform concat];

  // Draw every glyphs, moving right by the advance of each glyph.
  for (NSString *name in line) {
    [self drawGlyph:name position:position dirtyRect:dirtyRect];
    position.x += [_typeface advanceOfGlyphForName:name];
  }
  [NSGraphicsContext restoreGraphicsState];
}

- (void)drawGlyph:(NSString *)name
         position:(CGPoint)position
        dirtyRect:(CGRect)dirtyRect {
  [NSGraphicsContext saveGraphicsState];
  NSAffineTransform *transform = [NSAffineTransform transform];
  [transform translateXBy:position.x yBy:position.y];
  [transform concat];

  // Intersection test with a dirty rect and the bounds of this glyph outline.
  NSBezierPath *outline = [_typeface glyphOutlineForName:name];
  CGContextRef context = [NSGraphicsContext currentContext].CGContext;
  CGAffineTransform currentTransform = CGContextGetCTM(context);
  CGRect rect1 = CGRectApplyAffineTransform(dirtyRect, _transform);
  CGRect rect2 = CGRectApplyAffineTransform(outline.bounds, currentTransform);
  if (CGRectIntersectsRect(rect1, rect2)) {
    if (_outlined) {
      [self drawOutlineGlyph:outline];
    } else {
      [self drawSolidGlyph:outline];
    }
  }
  [NSGraphicsContext restoreGraphicsState];
}

- (void)drawSolidGlyph:(NSBezierPath *)outline {
  NSColor *foregroundColor;
  if (_inverted) {
    foregroundColor = [NSColor whiteColor];
  } else {
    foregroundColor = [NSColor blackColor];
  }
  [foregroundColor setFill];
  [outline fill];
}

- (void)drawOutlineGlyph:(NSBezierPath *)outline {
  // Scale line width by inverse of scroll view's magnification to get
  // consistent line width.
  NSAssert([self.superview.superview isKindOfClass:[NSScrollView class]], @"");
  NSScrollView *scrollView = (NSScrollView *)self.superview.superview;
  CGFloat scale = 1.0 / (_scale * scrollView.magnification);
  outline.lineWidth = scale;

  // Outline
  [[NSColor grayColor] setStroke];
  [outline stroke];

  // Control lines and points
  [[[NSColor grayColor] colorWithAlphaComponent:0.5] setStroke];
  [[NSColor grayColor] setFill];
  CGSize anchorSize = CGSizeMake(3.0 * scale, 3.0 * scale);
  CGPoint previous[3];
  CGPoint current[3];
  for (NSInteger index = 0; index < outline.elementCount; ++index) {
    NSBezierPathElement previousType =
        [outline elementAtIndex:index
               associatedPoints:previous];
    NSBezierPathElement currentType =
        [outline elementAtIndex:(index + 1) % outline.elementCount
               associatedPoints:current];
    switch (currentType) {
      case NSLineToBezierPathElement:
      case NSMoveToBezierPathElement:
        NSRectFill(CGRectMake(
            current[0].x - anchorSize.width / 2.0,
            current[0].y - anchorSize.height / 2.0,
            anchorSize.width,
            anchorSize.height));
        break;
      case NSCurveToBezierPathElement: {
        NSRectFill(CGRectMake(
            current[2].x - anchorSize.width / 2.0,
            current[2].y - anchorSize.height / 2.0,
            anchorSize.width,
            anchorSize.height));
        NSBezierPath *path;
        path = [NSBezierPath bezierPath];
        if (previousType == NSCurveToBezierPathElement) {
          [path moveToPoint:previous[2]];
        } else {
          [path moveToPoint:previous[0]];
        }
        [path lineToPoint:current[0]];
        path.lineWidth = scale;
        [path stroke];
        path = [NSBezierPath bezierPath];
        [path moveToPoint:current[1]];
        [path lineToPoint:current[2]];
        path.lineWidth = scale;
        [path stroke];
        NSRectFill(CGRectMake(
            current[0].x - anchorSize.width / 2.0,
            current[0].y - anchorSize.height / 2.0,
            anchorSize.width,
            anchorSize.height));
        NSRectFill(CGRectMake(
            current[1].x - anchorSize.width / 2.0,
            current[1].y - anchorSize.height / 2.0,
            anchorSize.width,
            anchorSize.height));
        break;
      }
      default:
        break;
    }
  }
}

#pragma mark Resizing

- (void)resizeToFitLines {
  CGSize size = [self sizeForLines:_lines];
  CGRect frame = self.frame;
  frame.size.width = ceil(size.width);
  frame.size.height = ceil(size.height);
  self.frame = frame;
}

- (CGSize)sizeForLines:(NSArray *)lines {
  CGSize size = CGSizeZero;
  size.height = _typeface.unitsPerEM * lines.count - _typeface.descender;
  CGFloat width;
  for (NSArray *line in lines) {
    width = 0.0;
    for (NSString *name in line) {
      width += [_typeface advanceOfGlyphForName:name];
      if (size.width < width) {
        size.width = width;
      }
    }
  }
  size.width *= _scale;
  size.height *= _scale;
  return size;
}

@end
