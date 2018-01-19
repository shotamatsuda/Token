// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

import AppKit

class ZeroSplitView : NSSplitView {
  override var dividerThickness: CGFloat {
    get {
      return 0.0
    }
  }
}
