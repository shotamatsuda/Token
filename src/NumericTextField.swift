// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

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
        name: NSControl.textDidEndEditingNotification,
        object: self)
  }

  private func setUpNumericTextField() {
    setUpTrackingArea()
    isEditable = false
    isSelectable = false
    NotificationCenter.default.addObserver(
        self,
        selector: #selector(controlTextDidEndEditing(_:)),
        name: NSControl.textDidEndEditingNotification,
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
    guard let info = infoForBinding(.value) else {
      return
    }
    guard let keyPath = info[.observedKeyPath] as? String else {
      return
    }
    guard let observedObject = info[.observedObject] as AnyObject? else {
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
      addCursorRect(bounds, cursor: .resizeLeftRight)
    }
  }

  override func controlTextDidEndEditing(_ notification: Notification) {
    guard let info = infoForBinding(.value) else {
      return
    }
    guard let keyPath = info[.observedKeyPath] as? String else {
      return
    }
    guard let observedObject = info[.observedObject] as AnyObject? else {
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
