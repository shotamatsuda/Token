//
//  TKNNSBezierPath+RoundedRect.m
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
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRETKN OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNETKN FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
//  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.
//

#import "TKNNSBezierPath+RoundedRect.h"

@implementation NSBezierPath (TKNRoundedRect)

+ (NSBezierPath *)bezierPathWithRoundedRect:(NSRect)rect
                          byRoundingCorners:(TKNRectCorner)corners
                               cornerRadius:(CGFloat)cornerRadius {
  NSGraphicsContext *context = [NSGraphicsContext currentContext];
  NSBezierPath *bezierPath = [self bezierPath];
  cornerRadius = MIN(cornerRadius, 0.5 * MIN(NSWidth(rect), NSHeight(rect)));
  NSRect insetRect = NSInsetRect(rect, cornerRadius, cornerRadius);
  BOOL flipped = [context isFlipped];
  NSPoint cornerPoint;
  if ((!flipped && corners & TKNRectCornerBottomLeft) ||
      (flipped && corners & TKNRectCornerTopLeft)) {
    cornerPoint = NSMakePoint(NSMinX(insetRect), NSMinY(insetRect));
    [bezierPath appendBezierPathWithArcWithCenter:cornerPoint
                                           radius:cornerRadius
                                       startAngle:180.0
                                         endAngle:270.0];
  } else {
    cornerPoint = NSMakePoint(NSMinX(rect), NSMinY(rect));
    [bezierPath appendBezierPathWithPoints:&cornerPoint count:1];
  }
  if ((!flipped && corners & TKNRectCornerBottomRight) ||
      (flipped && corners & TKNRectCornerTopRight)) {
    cornerPoint = NSMakePoint(NSMaxX(insetRect), NSMinY(insetRect));
    [bezierPath appendBezierPathWithArcWithCenter:cornerPoint
                                           radius:cornerRadius
                                       startAngle:270.0
                                         endAngle:360.0];
  } else {
    cornerPoint = NSMakePoint(NSMaxX(rect), NSMinY(rect));
    [bezierPath appendBezierPathWithPoints:&cornerPoint count:1];
  }
  if ((!flipped && corners & TKNRectCornerTopRight) ||
      (flipped && corners & TKNRectCornerBottomRight)) {
    cornerPoint = NSMakePoint(NSMaxX(insetRect), NSMaxY(insetRect));
    [bezierPath appendBezierPathWithArcWithCenter:cornerPoint
                                           radius:cornerRadius
                                       startAngle:0.0
                                         endAngle:90.0];
  } else {
    cornerPoint = NSMakePoint(NSMaxX(rect), NSMaxY(rect));
    [bezierPath appendBezierPathWithPoints:&cornerPoint count:1];
  }
  if ((!flipped && corners & TKNRectCornerTopLeft) ||
      (flipped && corners & TKNRectCornerBottomLeft)) {
    cornerPoint = NSMakePoint(NSMinX(insetRect), NSMaxY(insetRect));
    [bezierPath appendBezierPathWithArcWithCenter:cornerPoint
                                           radius:cornerRadius
                                       startAngle:90.0
                                         endAngle:180.0];
  } else {
    cornerPoint = NSMakePoint(NSMinX(rect), NSMaxY(rect));
    [bezierPath appendBezierPathWithPoints:&cornerPoint count:1];
  }
  [bezierPath closePath];
  return bezierPath;
}

+ (NSBezierPath *)bezierPathWithRoundedRect:(NSRect)rect
                               cornerRadius:(CGFloat)cornerRadius {
  return [NSBezierPath bezierPathWithRoundedRect:rect
                               byRoundingCorners:TKNRectCornerAllCorners
                                    cornerRadius:cornerRadius];
}

@end
