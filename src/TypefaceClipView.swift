//
//  The MIT License
//
//  Copyright (C) 2015-2017 Shota Matsuda
//
//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
//  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.
//

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
