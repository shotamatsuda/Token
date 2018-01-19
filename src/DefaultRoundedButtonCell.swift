// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

import AppKit

class DefaultRoundedButtonCell : RoundedButtonCell {
  override var textColor: NSColor {
    get {
      return NSColor.black.withAlphaComponent(0.75)
    }
  }

  override var highlightedTextColor: NSColor {
    get {
      return NSColor.black
    }
  }

  override var frameColor: NSColor {
    get {
      return NSColor.white.withAlphaComponent(0.75)
    }
  }

  override var highlightedFrameColor: NSColor {
    get {
      return NSColor.white
    }
  }
}
