//
//  NSBezierPath+RoundedRect.swift
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

import AppKit

extension NSBezierPath {
  convenience init(roundedRect: CGRect, radius: CGFloat) {
    self.init(roundedRect: roundedRect, corners: .All, radius: radius)
  }

  convenience init(roundedRect: CGRect, corners: RectCorners, radius: CGFloat) {
    self.init()
    guard let context = NSGraphicsContext.currentContext() else {
      return
    }
    let radius = min(radius, min(roundedRect.width, roundedRect.height) / 2.0)
    let insetRect = roundedRect.insetBy(dx: radius, dy: radius)
    // Bottom left
    if (!context.flipped && corners.contains(.BottomLeft)) ||
        (context.flipped && corners.contains(.TopLeft)) {
      appendBezierPathWithArcWithCenter(
          CGPoint(x: insetRect.minX, y: insetRect.minY),
          radius: radius,
          startAngle: 180.0,
          endAngle: 270.0)
    } else {
      var points = [CGPoint(x: roundedRect.minX, y: roundedRect.minY)]
      appendBezierPathWithPoints(&points, count: 1)
    }
    // Bottom right
    if (!context.flipped && corners.contains(.BottomRight)) ||
        (context.flipped && corners.contains(.TopRight)) {
      appendBezierPathWithArcWithCenter(
          CGPoint(x: insetRect.maxX, y: insetRect.minY),
          radius: radius,
          startAngle: 270.0,
          endAngle: 360.0)
    } else {
      var points = [CGPoint(x: roundedRect.maxX, y: roundedRect.minY)]
      appendBezierPathWithPoints(&points, count: 1)
    }
    // Top right
    if (!context.flipped && corners.contains(.TopRight)) ||
        (context.flipped && corners.contains(.BottomRight)) {
      appendBezierPathWithArcWithCenter(
          CGPoint(x: insetRect.maxX, y: insetRect.maxY),
          radius: radius,
          startAngle: 0.0,
          endAngle: 90.0)
    } else {
      var points = [CGPoint(x: roundedRect.maxX, y: roundedRect.maxY)]
      appendBezierPathWithPoints(&points, count: 1)
    }
    // Top left
    if (!context.flipped && corners.contains(.TopLeft)) ||
        (context.flipped && corners.contains(.BottomLeft)) {
      appendBezierPathWithArcWithCenter(
          CGPoint(x: insetRect.minX, y: insetRect.maxY),
          radius: radius,
          startAngle: 90.0,
          endAngle: 180.0)
    } else {
      var points = [CGPoint(x: roundedRect.minX, y: roundedRect.maxY)]
      appendBezierPathWithPoints(&points, count: 1)
    }
    closePath()
  }
}
