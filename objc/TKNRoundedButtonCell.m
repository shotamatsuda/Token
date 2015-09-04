//
//  TKNRoundedButtonCell.m
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

#import "TKNRoundedButtonCell.h"

#import "TKNNSBezierPath+RoundedRect.h"

@implementation TKNRoundedButtonCell

- (void)drawBezelWithFrame:(NSRect)frame inView:(NSView *)controlView {
  [NSGraphicsContext saveGraphicsState];
  NSBezierPath *path = [NSBezierPath
      bezierPathWithRoundedRect:frame
      cornerRadius:5.0];
  if (self.highlighted) {
    [[[NSColor whiteColor] colorWithAlphaComponent:0.2] setFill];
  } else {
    [[[NSColor whiteColor] colorWithAlphaComponent:0.1] setFill];
  }
  [path fill];
  [NSGraphicsContext restoreGraphicsState];
}

- (NSAttributedString *)attributedTitle {
  NSMutableAttributedString *title = [[NSMutableAttributedString alloc]
      initWithAttributedString:super.attributedTitle];
  NSRange range = NSMakeRange(0, title.length);
  [title beginEditing];
  [title addAttribute:NSBaselineOffsetAttributeName
                value:@2.0
                range:range];
  if (self.enabled) {
    if (self.highlighted) {
      [title addAttribute:NSForegroundColorAttributeName
                    value:[NSColor whiteColor]
                    range:range];
    } else {
      [title addAttribute:NSForegroundColorAttributeName
                    value:[[NSColor whiteColor] colorWithAlphaComponent:0.9]
                    range:range];
    }
  }
  [title endEditing];
  return title;
}

@end
