// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

import AppKit

class SettingsViewController : NSViewController {
  @objc var typeface: Typeface? {
    willSet {
      willChangeValue(forKey: "typeface")
    }

    didSet {
      didChangeValue(forKey: "typeface")
    }
  }
}
