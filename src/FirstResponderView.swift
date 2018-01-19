// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

import AppKit

class FirstResponderView : NSView {
  override var acceptsFirstResponder: Bool {
    get {
      return true
    }
  }
}
