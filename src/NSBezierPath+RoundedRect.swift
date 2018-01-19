// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

import AppKit

extension NSBezierPath {
  convenience init(roundedRect: CGRect, radius: CGFloat) {
    self.init(roundedRect: roundedRect, corners: .all, radius: radius)
  }

  convenience init(roundedRect: CGRect, corners: RectCorners, radius: CGFloat) {
    self.init()
    guard let context = NSGraphicsContext.current else {
      return
    }
    let radius = min(radius, min(roundedRect.width, roundedRect.height) / 2.0)
    let insetRect = roundedRect.insetBy(dx: radius, dy: radius)
    // Bottom left
    if (!context.isFlipped && corners.contains(.bottomLeft)) ||
        (context.isFlipped && corners.contains(.topLeft)) {
      appendArc(
          withCenter: CGPoint(x: insetRect.minX, y: insetRect.minY),
          radius: radius,
          startAngle: 180.0,
          endAngle: 270.0)
    } else {
      var points = [CGPoint(x: roundedRect.minX, y: roundedRect.minY)]
      appendPoints(&points, count: 1)
    }
    // Bottom right
    if (!context.isFlipped && corners.contains(.bottomRight)) ||
        (context.isFlipped && corners.contains(.topRight)) {
      appendArc(
          withCenter: CGPoint(x: insetRect.maxX, y: insetRect.minY),
          radius: radius,
          startAngle: 270.0,
          endAngle: 360.0)
    } else {
      var points = [CGPoint(x: roundedRect.maxX, y: roundedRect.minY)]
      appendPoints(&points, count: 1)
    }
    // Top right
    if (!context.isFlipped && corners.contains(.topRight)) ||
        (context.isFlipped && corners.contains(.bottomRight)) {
      appendArc(
          withCenter: CGPoint(x: insetRect.maxX, y: insetRect.maxY),
          radius: radius,
          startAngle: 0.0,
          endAngle: 90.0)
    } else {
      var points = [CGPoint(x: roundedRect.maxX, y: roundedRect.maxY)]
      appendPoints(&points, count: 1)
    }
    // Top left
    if (!context.isFlipped && corners.contains(.topLeft)) ||
        (context.isFlipped && corners.contains(.bottomLeft)) {
      appendArc(
          withCenter: CGPoint(x: insetRect.minX, y: insetRect.maxY),
          radius: radius,
          startAngle: 90.0,
          endAngle: 180.0)
    } else {
      var points = [CGPoint(x: roundedRect.minX, y: roundedRect.maxY)]
      appendPoints(&points, count: 1)
    }
    close()
  }
}
