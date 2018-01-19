// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

import AppKit

class WelcomeWindow : NSWindow {
  override var canBecomeKey: Bool {
    get {
      return true
    }
  }
}
