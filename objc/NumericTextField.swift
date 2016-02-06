//
//  NumericTextField.swift
//
//  The MIT License
//
//  Copyright (C) 2015 Shota Matsuda
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
  @IBInspectable var step = Double()

  override init(frame frameRect: NSRect) {
    super.init(frame: frameRect)
    setUpNumericTextField()
  }

  required init?(coder: NSCoder) {
    super.init(coder: coder)
    setUpNumericTextField()
  }

  deinit {
    NSNotificationCenter.defaultCenter().removeObserver(
        self,
        name: NSControlTextDidEndEditingNotification,
        object: self)
  }

  private func setUpNumericTextField() {
    setUpTrackingArea()
    editable = false
    selectable = false
    NSNotificationCenter.defaultCenter().addObserver(
        self,
        selector: "controlTextDidEndEditing:",
        name: NSControlTextDidEndEditingNotification,
        object: self)
  }

  private var trackingArea: NSTrackingArea?

  private func setUpTrackingArea() {
    if let trackingArea = trackingArea {
      removeTrackingArea(trackingArea)
    }
    trackingArea = NSTrackingArea(
        rect: bounds,
        options: [.MouseEnteredAndExited, .MouseMoved, .ActiveInKeyWindow],
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

  override var mouseDownCanMoveWindow: Bool {
    get {
      return true
    }
  }

  private var initialLocation: CGPoint?
  private var initialValue: Double?

  override func mouseDown(event: NSEvent) {
    super.mouseDown(event)
    initialLocation = event.locationInWindow
    initialValue = doubleValue
  }

  override func mouseDragged(event: NSEvent) {
    super.mouseDragged(event)
    guard let initialLocation = initialLocation,
        initialValue = initialValue else {
      return
    }
    let location = event.locationInWindow
    let delta = round((location.x - initialLocation.x) / 2.0).native * step
    let result = initialValue + delta

    // Propagate the change through binding, and store the value after
    // propagation back to the double value of the text field.
    guard let bindingInfo = infoForBinding(NSValueBinding),
        observedObject = bindingInfo[NSObservedObjectKey],
        keyPath = bindingInfo[NSObservedKeyPathKey] as? String else {
      return
    }
    observedObject.setValue?(NSNumber(double: result), forKeyPath: keyPath)
    if let value = observedObject.valueForKeyPath(keyPath) as? NSNumber {
      doubleValue = value.doubleValue
    }
  }

  override func mouseUp(event: NSEvent) {
    super.mouseUp(event)
    guard event.clickCount != 0 else {
      return
    }
    guard let window = window else {
      return
    }
    let location = convertPoint(
        event.locationInWindow,
        fromView: window.contentView)
    if CGRectContainsPoint(bounds, location) {
      editable = true
      selectable = true
      selectText(self)
      resetCursorRects()
    }
  }

  override func resetCursorRects() {
    if editable {
      super.resetCursorRects()
    } else {
      addCursorRect(bounds, cursor: NSCursor.resizeLeftRightCursor())
    }
  }

  override func controlTextDidEndEditing(notification: NSNotification) {
    guard let bindingInfo = infoForBinding(NSValueBinding),
        observedObject = bindingInfo[NSObservedObjectKey],
        keyPath = bindingInfo[NSObservedKeyPathKey] as? String else {
      return
    }
    if let value = observedObject.valueForKeyPath(keyPath) as? NSNumber {
      doubleValue = value.doubleValue
      editable = false
      selectable = false
      resetCursorRects()
    }
  }
}
