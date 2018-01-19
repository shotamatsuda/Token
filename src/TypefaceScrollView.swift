// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

import AppKit

class TypefaceScrollView : NSScrollView {
  override var mouseDownCanMoveWindow: Bool {
    get {
      return true
    }
  }
}
