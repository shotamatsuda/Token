// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

import AppKit

class ApplicationDelegate : NSObject, NSApplicationDelegate {
  func applicationShouldTerminateAfterLastWindowClosed(
      _ sender: NSApplication) -> Bool {
    return true
  }
}
