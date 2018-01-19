// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

import AppKit

class ProgressViewController : NSViewController {
  @IBOutlet var progressIndicator: NSProgressIndicator?
  @IBOutlet var progressLabel: NSTextField?

  override func viewDidAppear() {
    super.viewDidAppear()
    progressIndicator?.isIndeterminate = true
    progressIndicator?.startAnimation(self)
  }

  @IBAction func cancel(_ sender: AnyObject?) {
    guard let window = view.window, let sheetParent = window.sheetParent else {
      return
    }
    sheetParent.endSheet(window, returnCode: .cancel)
  }
}
