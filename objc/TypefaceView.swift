//
//  TypefaceView.swift
//
//  The MIT License
//
//  Copyright (C) 2015-2016 Shota Matsuda
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

@objc(TKNTypefaceView)
class TypefaceView : NSView {
  var typeface: TKNTypeface? {
    didSet {
      if typeface != oldValue {
        needsDisplay = true
      }
    }
  }

  var inverted: Bool = false {
    didSet {
      if inverted != oldValue {
        needsDisplay = true
      }
    }
  }

  var outlined: Bool = false {
    didSet {
      if outlined != oldValue {
        needsDisplay = true
      }
    }
  }

  private var lines: [[String]] = [
    ["A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M"],
    ["N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z"],
    ["one", "two", "three", "four", "five",
     "six", "seven", "eight", "nine", "zero"]
  ]
  private var scale: CGFloat = 0.085
  private var transform: CGAffineTransform = CGAffineTransform()

  override var acceptsFirstResponder: Bool {
    get {
      return true
    }
  }

  // MARK: Drawing

  override func drawRect(dirtyRect: NSRect) {
    resizeToFitLines()

    // Remember this view's current transformation matrix because it's not
    // identity matrix. We'll use this later for intersection tests.
    guard let currentContext = NSGraphicsContext.currentContext() else {
      return
    }
    self.transform = CGContextGetCTM(currentContext.CGContext)

    // Background color
    NSGraphicsContext.saveGraphicsState()
    defer {
      NSGraphicsContext.restoreGraphicsState()
    }
    var backgroundColor: NSColor?
    if inverted {
      backgroundColor = NSColor.blackColor()
    } else {
      backgroundColor = NSColor.whiteColor()
    }
    backgroundColor!.setFill()
    NSRectFill(dirtyRect)

    // Move the origin to horizontal center and vertical top.
    let transform = NSAffineTransform()
    transform.translateXBy(frame.width / 2.0, yBy: frame.height)
    transform.scaleBy(scale)
    transform.concat()

    // Draw every lines, moving down by ascender + descender per line.
    guard let typeface = typeface else {
      return
    }
    var position = CGPoint()
    position.y -= CGFloat(typeface.ascender - typeface.descender)
    for line in lines {
      drawLine(line, position: position, dirtyRect: dirtyRect)
      position.x = 0.0
      position.y -= CGFloat(typeface.ascender - typeface.descender)
    }
  }

  private func drawLine(
      line: Array<String>,
      var position: CGPoint,
      dirtyRect: CGRect) {
    guard let typeface = typeface else {
      return
    }
    // Derive the sum of advances in this line for centering.
    var lineWidth = CGFloat()
    for name in line {
      lineWidth += CGFloat(typeface.advanceOfGlyphForName(name))
    }
    NSGraphicsContext.saveGraphicsState()
    defer {
      NSGraphicsContext.restoreGraphicsState()
    }
    let transform = NSAffineTransform()
    transform.translateXBy(-lineWidth / 2.0, yBy: 0.0)
    transform.concat()

    // Draw every glyphs, moving right by the advance of each glyph.
    for name in line {
      drawGlyph(name, position: position, dirtyRect: dirtyRect)
      position.x += CGFloat(typeface.advanceOfGlyphForName(name))
    }
  }

  private func drawGlyph(name: String, position: CGPoint, dirtyRect: CGRect) {
    guard let typeface = typeface,
      currentContext = NSGraphicsContext.currentContext() else {
      return
    }
    NSGraphicsContext.saveGraphicsState()
    defer {
      NSGraphicsContext.restoreGraphicsState()
    }
    let transform = NSAffineTransform()
    transform.translateXBy(position.x, yBy: position.y)
    transform.concat()

    // Intersection test with a dirty rect and the bounds of this glyph outline.
    let bounds = typeface.boundingRectOfGlyphForName(name)
    let context = currentContext.CGContext
    let currentTransform = CGContextGetCTM(context)
    let rect1 = CGRectApplyAffineTransform(dirtyRect, self.transform)
    var rect2 = CGRectApplyAffineTransform(bounds, currentTransform)
    // Inset by the size of path handles
    rect2.insetInPlace(dx: -4.0, dy: -4.0)
    guard rect1.intersects(rect2) else {
      return
    }
    if let outline = typeface.glyphOutlineForName(name) {
      if outlined {
        drawOutlineGlyph(outline)
      } else {
        drawSolidGlyph(outline)
      }
    }
  }

  private func drawSolidGlyph(outline: NSBezierPath) {
    var foregroundColor: NSColor?
    if inverted {
      foregroundColor = NSColor.whiteColor()
    } else {
      foregroundColor = NSColor.blackColor()
    }
    foregroundColor!.setFill()
    outline.fill()
  }

  private func drawOutlineGlyph(outline: NSBezierPath) {
    // Scale line width by inverse of scroll view's magnification to get
    // consistent line width.
    let scrollView = superview!.superview as! NSScrollView
    let scale = 1.0 / (self.scale * scrollView.magnification)
    outline.lineWidth = scale

    // Outline 
    NSColor.grayColor().colorWithAlphaComponent(0.5).setStroke()
    outline.stroke()

    // Control lines and points
    NSColor.grayColor().setFill()
    let pointRect = CGRect(
        x: -2.0 * scale,
        y: -2.0 * scale,
        width: 4.0 * scale,
        height: 4.0 * scale)
    var previous = Array<CGPoint>(count: 3, repeatedValue: CGPoint())
    var current = Array<CGPoint>(count: 3, repeatedValue: CGPoint())
    for index in 0..<outline.elementCount {
      let previousType = outline.elementAtIndex(
          index,
          associatedPoints: &previous)
      let currentType = outline.elementAtIndex(
          (index + 1) % outline.elementCount,
          associatedPoints: &current)
      switch currentType {
      case .LineToBezierPathElement:
        fallthrough
      case .MoveToBezierPathElement:
        NSRectFill(CGRect(
            x: current[0].x + pointRect.minX,
            y: current[0].y + pointRect.minY,
            width: pointRect.width,
            height: pointRect.height))
      case .CurveToBezierPathElement:
        NSRectFill(CGRect(
            x: current[2].x + pointRect.minX,
            y: current[2].y + pointRect.minY,
            width: pointRect.width,
            height: pointRect.height))
        var path = NSBezierPath()
        if previousType == .CurveToBezierPathElement {
          path.moveToPoint(previous[2])
        } else {
          path.moveToPoint(previous[0])
        }
        path.lineToPoint(current[0])
        path.lineWidth = scale
        path.stroke()
        path = NSBezierPath()
        path.moveToPoint(current[1])
        path.lineToPoint(current[2])
        path.lineWidth = scale
        path.stroke()
        path = NSBezierPath(ovalInRect: CGRect(
            x: current[0].x + pointRect.minX,
            y: current[0].y + pointRect.minY,
            width: pointRect.width,
            height: pointRect.height))
        path.lineWidth = scale
        path.stroke()
        path = NSBezierPath(ovalInRect: CGRect(
            x: current[1].x + pointRect.minX,
            y: current[1].y + pointRect.minY,
            width: pointRect.width,
            height: pointRect.height))
        path.lineWidth = scale
        path.stroke()
      default:
        break
      }
    }
  }

  // MARK: Resizing

  private func resizeToFitLines() {
    let size = sizeForLines(lines)
    frame.size = CGSize(width: ceil(size.width), height: ceil(size.height))
  }

  private func sizeForLines(lines: Array<Array<String>>) -> CGSize {
    guard let typeface = typeface else {
      return CGSize()
    }
    let lineHeight = typeface.ascender - typeface.descender
    var size = CGSize()
    size.height = CGFloat(lineHeight * lines.count - typeface.descender)
    for line in lines {
      var width = CGFloat()
      for name in line {
        width += CGFloat(typeface.advanceOfGlyphForName(name))
        if size.width < width {
          size.width = width
        }
      }
    }
    size.width += CGFloat(lineHeight) / 2.0
    size.width *= scale
    size.height *= scale
    return size
  }
}
