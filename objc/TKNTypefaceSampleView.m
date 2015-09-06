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

- (CGSize)sizeForLines:(NSArray *)lines;
- (void)drawLine:(NSArray *)glyphNames
        position:(CGPoint)position
         control:(BOOL)control;
- (void)drawGlyph:(NSString *)glyphName
         position:(CGPoint)position
          control:(BOOL)control;

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

#pragma mark Drawing

- (void)drawRect:(NSRect)rect {
  CGSize size = [self sizeForLines:_lines];
  CGRect frame = self.frame;
  frame.size.width = ceil(size.width);
  frame.size.height = ceil(size.height);
  self.frame = frame;
  [NSGraphicsContext saveGraphicsState];
  NSColor *backgroundColor;
  if (_inverted) {
    backgroundColor = [NSColor blackColor];
  } else {
    backgroundColor = [NSColor whiteColor];
  }
  [backgroundColor setFill];
  NSRectFill(rect);
  NSAffineTransform *transform = [NSAffineTransform transform];
  [transform translateXBy:size.width / 2.0 yBy:size.height];
  [transform scaleBy:_scale];
  [transform concat];
  CGPoint position = CGPointZero;
  position.y -= _typeface.ascender - _typeface.descender;
  for (NSArray *line in _lines) {
    [self drawLine:line position:position control:NO];
    position.x = 0.0;
    position.y -= _typeface.ascender - _typeface.descender;
  }
  [NSGraphicsContext restoreGraphicsState];
}

- (void)drawLine:(NSArray *)line
        position:(CGPoint)position
         control:(BOOL)control {
  CGFloat advances = 0.0;
  for (NSString *name in line) {
    advances += [_typeface advanceOfGlyphForName:name];
  }
  [NSGraphicsContext saveGraphicsState];
  NSAffineTransform *transform = [NSAffineTransform transform];
  [transform translateXBy:-advances / 2.0 yBy:0.0];
  [transform concat];
  for (NSString *name in line) {
    [self drawGlyph:name position:position control:control];
    position.x += [_typeface advanceOfGlyphForName:name];
  }
  [NSGraphicsContext restoreGraphicsState];
  if (self.frame.size.width < position.x * _scale) {
    CGRect frame = self.frame;
    frame.size.width = ceil(position.x * _scale);
    self.frame = frame;
  }
}

- (void)drawGlyph:(NSString *)name
         position:(CGPoint)position
          control:(BOOL)control {
  [NSGraphicsContext saveGraphicsState];
  NSColor *foregroundColor;
  if (_inverted) {
    foregroundColor = [NSColor whiteColor];
  } else {
    foregroundColor = [NSColor blackColor];
  }
  NSAffineTransform *transform = [NSAffineTransform transform];
  [transform translateXBy:position.x yBy:position.y];
  [transform concat];
  if (control) {
    NSBezierPath *path = [_typeface glyphOutlineForName:name];
    NSBezierPath *outline = [NSBezierPath bezierPath];
    for (NSInteger index = 0; index < path.elementCount; ++index) {
      CGPoint points[3];
      NSBezierPathElement type = [path elementAtIndex:index
                                     associatedPoints:points];
      switch (type) {
        case NSMoveToBezierPathElement:
          [outline moveToPoint:points[0]];
          break;
        case NSLineToBezierPathElement:
          [outline lineToPoint:points[0]];
          break;
        case NSCurveToBezierPathElement:
          [outline curveToPoint:points[2]
                  controlPoint1:points[0]
                  controlPoint2:points[1]];
          break;
        case NSClosePathBezierPathElement:
          [outline closePath];
          break;
        default:
          break;
      }
    }
    [foregroundColor set];
    [outline stroke];
  } else {
    [foregroundColor setFill];
    [[_typeface glyphOutlineForName:name] fill];
  }
  [NSGraphicsContext restoreGraphicsState];
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
