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

class NumericTextField : NSTextField {
  @IBInspectable var step: Double = Double()

  override init(frame frameRect: NSRect) {
    super.init(frame: frameRect)
    setUpNumericTextField()
  }

  required init?(coder: NSCoder) {
    super.init(coder: coder)
    setUpNumericTextField()
  }

  deinit {
    NotificationCenter.default.removeObserver(
        self,
        name: NSNotification.Name.NSControlTextDidEndEditing,
        object: self)
  }

  private func setUpNumericTextField() {
    setUpTrackingArea()
    isEditable = false
    isSelectable = false
    NotificationCenter.default.addObserver(
        self,
        selector: #selector(controlTextDidEndEditing(_:)),
        name: NSNotification.Name.NSControlTextDidEndEditing,
        object: self)
  }

  private var trackingArea: NSTrackingArea?

  private func setUpTrackingArea() {
    if let trackingArea = trackingArea {
      removeTrackingArea(trackingArea)
    }
    trackingArea = NSTrackingArea(
        rect: bounds,
        options: [.mouseEnteredAndExited, .mouseMoved, .activeInKeyWindow],
        owner: self,
        userInfo: nil)
  }

  override var frame: CGRect {
    didSet {
      if frame != oldValue {
        setUpTrackingArea()
      }
    }
  }

  override func mouseDragged(with event: NSEvent) {
    super.mouseDragged(with: event)

    // Propagate the change through binding, and store the value after
    // propagation back to the double value of the text field.
    guard let info = infoForBinding(NSValueBinding) else {
      return
    }
    guard let keyPath = info[NSObservedKeyPathKey] as? String else {
      return
    }
    guard let observedObject = info[NSObservedObjectKey] as AnyObject? else {
      return
    }
    let result = doubleValue + Double(event.deltaX) * step
    observedObject.setValue(NSNumber(value: result), forKeyPath: keyPath)
    if let value = observedObject.value(forKeyPath: keyPath) as? NSNumber {
      doubleValue = value.doubleValue
    }
  }

  override func mouseUp(with event: NSEvent) {
    super.mouseUp(with: event)
    guard event.clickCount != 0 else {
      return
    }
    guard let window = window else {
      return
    }
    let location = convert(event.locationInWindow, from: window.contentView)
    if bounds.contains(location) {
      isEditable = true
      isSelectable = true
      selectText(self)
      resetCursorRects()
    }
  }

  override func resetCursorRects() {
    if isEditable {
      super.resetCursorRects()
    } else {
      addCursorRect(bounds, cursor: NSCursor.resizeLeftRight())
    }
  }

  override func controlTextDidEndEditing(_ notification: Notification) {
    guard let info = infoForBinding(NSValueBinding) else {
      return
    }
    guard let keyPath = info[NSObservedKeyPathKey] as? String else {
      return
    }
    guard let observedObject = info[NSObservedObjectKey] as AnyObject? else {
      return
    }
    if let value = observedObject.value(forKeyPath: keyPath) as? NSNumber {
      doubleValue = value.doubleValue
      isEditable = false
      isSelectable = false
      resetCursorRects()
    }
  }
}
