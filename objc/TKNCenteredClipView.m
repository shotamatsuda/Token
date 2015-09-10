//
//  TKNCenteredClipView.m
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

#import "TKNCenteredClipView.h"

@interface TKNCenteredClipView ()

@property (nonatomic, assign) CGSize documentSize;

@end

@implementation TKNCenteredClipView

-(void)viewFrameChanged:(NSNotification *)notification {
  CGRect bounds = self.bounds;
  [super viewFrameChanged:notification];
  CGSize documentSize = [self.documentView frame].size;
  CGPoint point = bounds.origin;
  if (_documentSize.width && bounds.size.width < documentSize.width) {
    CGFloat scale = documentSize.width / _documentSize.width;
    point.x = CGRectGetMidX(bounds) * scale - bounds.size.width / 2.0;
  }
  if (_documentSize.height && bounds.size.height < documentSize.height) {
    CGFloat scale = documentSize.height / _documentSize.height;
    point.y = CGRectGetMidY(bounds) * scale - bounds.size.height / 2.0;
  }
  if (!CGPointEqualToPoint(point, bounds.origin)) {
    bounds.origin = point;
    self.bounds = bounds;
  }
  _documentSize = documentSize;
}

- (CGRect)constrainBoundsRect:(CGRect)proposedBounds {
  CGRect bounds = [super constrainBoundsRect:proposedBounds];
  CGRect documentFrame = [self.documentView frame];
  if (proposedBounds.size.width >= documentFrame.size.width) {
    bounds.origin.x = (proposedBounds.size.width -
                       documentFrame.size.width) / -2.0;
  }
  if (proposedBounds.size.height >= documentFrame.size.height) {
    bounds.origin.y = (proposedBounds.size.height -
                       documentFrame.size.height) / -2.0;
  }
  return bounds;
}

@end
