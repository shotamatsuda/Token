//
//  The MIT License
//
//  Copyright (C) 2015-2017 Shota Matsuda
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

class RoundedButtonCell : NSButtonCell {
  var textColor: NSColor {
    get {
      return NSColor.white.withAlphaComponent(0.9)
    }
  }

  var highlightedTextColor: NSColor {
    get {
      return NSColor.white
    }
  }

  var frameColor: NSColor {
    get {
      return NSColor.white.withAlphaComponent(0.1)
    }
  }

  var highlightedFrameColor: NSColor {
    get {
      return NSColor.white.withAlphaComponent(0.2)
    }
  }

  override var attributedTitle: NSAttributedString {
    get {
      if !isEnabled {
        return super.attributedTitle
      }
      let title = NSMutableAttributedString(
          attributedString: super.attributedTitle)
      let range = NSRange(location: 0, length: title.length)
      title.beginEditing()
      if isHighlighted {
        title.addAttribute(
            NSForegroundColorAttributeName,
            value: highlightedTextColor,
            range: range)
      } else {
        title.addAttribute(
            NSForegroundColorAttributeName,
            value: textColor,
            range: range)
      }
      title.endEditing()
      return title
    }

    set(value) {
      super.attributedTitle = value
    }
  }

  override func drawBezel(withFrame frame: NSRect, in controlView: NSView) {
    NSGraphicsContext.saveGraphicsState()
    defer {
      NSGraphicsContext.restoreGraphicsState()
    }
    let path = NSBezierPath(roundedRect: frame, radius: 4.0)
    if isHighlighted {
      highlightedFrameColor.setFill()
    } else {
      frameColor.setFill()
    }
    path.fill()
  }
}
