//
//  TKNMultilineTextField.m
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

#import "TKNMultilineTextField.h"

@interface TKNMultilineTextField ()

@property (nonatomic, assign) CGSize previousIntrinsicContentSize;

@end

@implementation TKNMultilineTextField

- (NSSize)intrinsicContentSize {
  if ([self.cell wraps] && self.frame.size.height > 1.0) {
    return [self.cell cellSizeForBounds:
        NSMakeRect(0.0, 0.0, self.bounds.size.width, CGFLOAT_MAX)];
  }
  return super.intrinsicContentSize;
}

- (void) layout {
  [super layout];
  [self invalidateWordWrappedContentSizeIfNeeded];
}

- (void)setFrameSize:(NSSize)size {
  [super setFrameSize:size];
  [self invalidateWordWrappedContentSizeIfNeeded];
}

- (void)invalidateWordWrappedContentSizeIfNeeded {
  CGSize intrinsicContentSize = self.intrinsicContentSize;
  if (!CGSizeEqualToSize(_previousIntrinsicContentSize, intrinsicContentSize)) {
    [self invalidateIntrinsicContentSize];
  }
  _previousIntrinsicContentSize = intrinsicContentSize;
}
@end
