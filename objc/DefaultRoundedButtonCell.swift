//
//  DefaultRoundedButtonCell.swift
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

class DefaultRoundedButtonCell : NSButtonCell {
  override var attributedTitle: NSAttributedString {
    get {
      if !enabled {
        return super.attributedTitle
      }
      guard let appearance = controlView?.effectiveAppearance else {
        return super.attributedTitle
      }
      var color: NSColor
      var highlightedColor: NSColor
      if appearance.name == NSAppearanceNameVibrantDark {
        highlightedColor = NSColor.blackColor()
        color = highlightedColor.colorWithAlphaComponent(0.75)
      } else {
        highlightedColor = NSColor.whiteColor()
        color = highlightedColor.colorWithAlphaComponent(0.75)
      }
      let title = NSMutableAttributedString(
          attributedString: super.attributedTitle)
      let range = NSRange(location: 0, length: title.length)
      title.beginEditing()
      if highlighted {
        title.addAttribute(
            NSForegroundColorAttributeName,
            value:highlightedColor,
            range:range)
      } else {
        title.addAttribute(
            NSForegroundColorAttributeName,
            value:color,
            range:range)
      }
      title.endEditing()
      return title
    }
    
    set(value) {
      super.attributedTitle = value
    }
  }

  override func drawBezelWithFrame(frame: NSRect, inView controlView: NSView) {
    guard let appearance = self.controlView?.effectiveAppearance else {
      return
    }
    NSGraphicsContext.saveGraphicsState()
    let path = NSBezierPath(roundedRect: frame, radius: 4.0)
    var color: NSColor
    var highlightedColor: NSColor;
    if appearance.name == NSAppearanceNameVibrantDark {
      highlightedColor = NSColor.whiteColor()
      color = highlightedColor.colorWithAlphaComponent(0.75)
    } else {
      highlightedColor = NSColor.alternateSelectedControlColor()
      color = highlightedColor.colorWithAlphaComponent(0.75)
    }
    if highlighted {
      highlightedColor.setFill()
    } else {
      color.setFill()
    }
    path.fill()
    NSGraphicsContext.restoreGraphicsState()
  }
}
