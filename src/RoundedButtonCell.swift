// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

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
            .foregroundColor,
            value: highlightedTextColor,
            range: range)
      } else {
        title.addAttribute(
            .foregroundColor,
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
