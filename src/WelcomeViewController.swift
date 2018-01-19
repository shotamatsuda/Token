// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

import AppKit

class WelcomeViewController : NSViewController {
  @IBAction func cancel(_ sender: AnyObject?) {
    guard let window = view.window, let sheetParent = window.sheetParent else {
      return
    }
    sheetParent.endSheet(window, returnCode: .cancel)
  }
}
