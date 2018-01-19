// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

import AppKit

class MultilineTextField : NSTextField {
  override var intrinsicContentSize: CGSize {
    get {
      guard let cell = cell else {
        return super.intrinsicContentSize
      }
      if cell.wraps && frame.height > 1.0 {
        return cell.cellSize(forBounds: CGRect(
            x: 0.0,
            y: 0.0,
            width: bounds.width,
            height: CGFloat.greatestFiniteMagnitude))
      }
      return super.intrinsicContentSize
    }
  }

  override func layout() {
    super.layout()
    invalidateIntrinsicContentSize()
  }
}
