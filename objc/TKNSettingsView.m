//
//  TKNSettingsView.m
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

#import "TKNSettingsView.h"

#import "TKNNSBezierPath+RoundedRect.h"

@implementation TKNSettingsView

- (void)setFrame:(NSRect)frame {
  BOOL changed = !CGRectEqualToRect(frame, self.bounds);
  [super setFrame:frame];
  if (changed) {
    self.maskImage = self.maskImage;
  }
}

- (NSImage *)maskImage {
  NSImage *image;
  if (_rounded) {
    image = [NSImage
        imageWithSize:self.bounds.size
        flipped:NO
        drawingHandler:^BOOL(NSRect rect) {
      NSBezierPath *path = [NSBezierPath
          bezierPathWithRoundedRect:rect
          byRoundingCorners:TKNRectCornerTopLeft
          cornerRadius:5.0];
      [[NSColor blackColor] setFill];
      [path fill];
      return YES;
    }];
  } else {
    image = [NSImage
        imageWithSize:self.bounds.size
        flipped:NO
        drawingHandler:^BOOL(NSRect rect) {
      NSBezierPath *path = [NSBezierPath bezierPathWithRect:rect];
      [[NSColor blackColor] setFill];
      [path fill];
      return YES;
    }];
  }
  return image;
}

- (void)setRounded:(BOOL)rounded {
  if (rounded != _rounded) {
    _rounded = rounded;
    self.maskImage = self.maskImage;
  }
}

@end
