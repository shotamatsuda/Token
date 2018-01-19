// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

import AppKit

class TypefaceClipView : NSClipView {
  override var mouseDownCanMoveWindow: Bool {
    get {
      return true
    }
  }

  private var documentSize: CGSize = CGSize()

  override func viewFrameChanged(_ notification: Notification) {
    super.viewFrameChanged(notification)
    guard let documentView = documentView else {
      return
    }
    let documentFrame = documentView.frame
    documentSize = documentFrame.size
    var point = bounds.origin
    if documentFrame.width > 0.0 && bounds.width < documentFrame.width {
      let scale = documentFrame.width / documentSize.width
      point.x = bounds.midX * scale - bounds.width / 2.0
    }
    if documentFrame.height > 0.0 && bounds.height < documentFrame.height {
      let scale = documentFrame.height / documentSize.height
      point.y = bounds.midY * scale - bounds.height / 2.0
    }
    if point == bounds.origin {
      bounds = CGRect(origin: point, size: bounds.size)
    }
  }

  override func constrainBoundsRect(_ proposedBounds: NSRect) -> NSRect {
    var bounds = super.constrainBoundsRect(proposedBounds)
    guard let documentView = documentView else {
      return bounds
    }
    let documentFrame = documentView.frame
    if proposedBounds.width >= documentFrame.width {
      bounds.origin.x = (proposedBounds.width - documentFrame.width) / -2.0
    }
    if proposedBounds.height >= documentFrame.height {
      bounds.origin.y = (proposedBounds.height - documentFrame.height) / -2.0
    }
    return bounds
  }
}
