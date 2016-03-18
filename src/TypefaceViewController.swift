//
//  TypefaceViewController.swift
//
//  The MIT License
//
//  Copyright (C) 2015-2016 Shota Matsuda
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

class TypefaceViewController : NSViewController {
  private let KVOContext = UnsafeMutablePointer<()>()

  @IBOutlet var typefaceView: TypefaceView?
  @IBOutlet var scrollView: NSScrollView?

  deinit {
    let defaultCenter = NSNotificationCenter.defaultCenter()
    defaultCenter.removeObserver(
        self,
        name: NSViewFrameDidChangeNotification,
        object: typefaceView)
    defaultCenter.removeObserver(
        self,
        name: NSViewFrameDidChangeNotification,
        object: scrollView)
  }

  override func viewDidLoad() {
    super.viewDidLoad()
    guard let scrollView = scrollView else {
      return
    }

    // Typeface view
    typefaceView = TypefaceView(frame: scrollView.frame)
    typefaceView?.typeface = typeface

    // Scroll view
    scrollView.autoresizingMask = [.ViewWidthSizable, .ViewHeightSizable]
    scrollView.hasHorizontalScroller = true
    scrollView.hasVerticalScroller = true
    scrollView.documentView = typefaceView
    scrollView.allowsMagnification = true
    scrollView.minMagnification = 0.03125
    scrollView.maxMagnification = 64.0

    // Observe for the frames' changes for zooming to fit
    let defaultCenter = NSNotificationCenter.defaultCenter()
    defaultCenter.addObserver(
        self,
        selector: "contentViewFrameDidChange:",
        name: NSViewFrameDidChangeNotification,
        object: typefaceView)
    defaultCenter.addObserver(
        self,
        selector: "contentViewFrameDidChange:",
        name: NSViewFrameDidChangeNotification,
        object: scrollView)

    // Inject self to the responder chain
    nextResponder = scrollView.contentView.nextResponder
    scrollView.contentView.nextResponder = self
  }

  override func scrollWheel(event: NSEvent) {
    guard let scrollView = scrollView else {
      return
    }
    if event.type == .ScrollWheel &&
        event.modifierFlags.contains(.AlternateKeyMask) {
      guard let window = view.window else {
        return
      }
      alwaysZoomsToFit = false
      let center = scrollView.contentView.convertPoint(
          event.locationInWindow,
          fromView: window.contentView)
      let scale = Double(event.scrollingDeltaY < 0.0 ? 0.9 : 1.0 / 0.9)
      willChangeValueForKey("magnification")
      magnificationValue = min(max(
          magnificationValue * scale,
          Double(scrollView.minMagnification)),
          Double(scrollView.maxMagnification))
      didChangeValueForKey("magnification")
      scrollView.setMagnification(
          CGFloat(magnificationValue),
          centeredAtPoint: center)
    } else {
      scrollView.scrollWheel(event)
    }
  }

  override func magnifyWithEvent(event: NSEvent) {
    guard let scrollView = scrollView, window = view.window else {
      return
    }
    alwaysZoomsToFit = false
    let center = scrollView.contentView.convertPoint(
        event.locationInWindow,
        fromView: window.contentView)
    willChangeValueForKey("magnification")
    magnificationValue = min(max(
        magnificationValue * (1.0 + Double(event.magnification)),
        Double(scrollView.minMagnification)),
        Double(scrollView.maxMagnification))
    didChangeValueForKey("magnification")
    scrollView.setMagnification(
        CGFloat(magnificationValue),
        centeredAtPoint: center)
  }

  override func observeValueForKeyPath(
      keyPath: String?,
      ofObject object: AnyObject?,
      change: [String : AnyObject]?,
      context: UnsafeMutablePointer<Void>) {
    if context == KVOContext {
      if object === typeface {
        typefaceView?.needsDisplay = true
      }
    } else {
      super.observeValueForKeyPath(
          keyPath,
          ofObject: object,
          change: change,
          context: context)
    }
  }

  func contentViewFrameDidChange(notification: NSNotification) {
    if alwaysZoomsToFit {
      zoomToFitAnimated(false)
    }
  }

  // MARK: Parameters

  private let typefaceKeyPaths = [
      "behavior", "strokeWidth", "capHeight",
      "strokeWidthUnit", "capHeightUnit"]

  var typeface: Typeface? {
    willSet {
      guard let typeface = typeface else {
        return
      }
      for keyPath in typefaceKeyPaths {
        typeface.removeObserver(self, forKeyPath: keyPath, context: KVOContext)
      }
    }

    didSet {
      guard let typeface = typeface else {
        return
      }
      if typeface !== oldValue {
        typefaceView?.typeface = typeface
        for keyPath in typefaceKeyPaths {
          typeface.addObserver(
              self,
              forKeyPath: keyPath,
              options: .New,
              context: KVOContext)
        }
      }
    }
  }

  var inverted: Bool = false {
    didSet {
      typefaceView?.inverted = inverted
      if inverted {
        scrollView?.backgroundColor = NSColor.blackColor()
      } else {
        scrollView?.backgroundColor = NSColor.whiteColor()
      }
    }
  }

  var outlined: Bool = false {
    didSet {
      typefaceView?.outlined = outlined
    }
  }

  @IBAction func toggleInverted(sender: AnyObject?) {
    inverted = !inverted
  }

  @IBAction func toggleOutlined(sender: AnyObject?) {
    outlined = !outlined
  }

  // MARK: Zooming

  private var magnificationValue: Double = 1.0
  private var magnificationQueue: Array<Double> = Array<Double>()

  var magnification: Double {
    get {
      return magnificationValue
    }

    set(value) {
      setMagnification(value, animated: false)
    }
  }

  var alwaysZoomsToFit: Bool = true {
    didSet {
      if alwaysZoomsToFit != oldValue && alwaysZoomsToFit {
        zoomToFitAnimated(true)
      }
    }
  }

  func setMagnification(magnification: Double, animated: Bool) {
    guard let scrollView = scrollView else {
      return
    }
    let value = min(max(
        magnification,
        Double(scrollView.minMagnification)),
        Double(scrollView.maxMagnification))
    if value == magnificationValue {
      return
    }
    willChangeValueForKey("magnification")
    magnificationValue = value
    didChangeValueForKey("magnification")
    if animated {
      magnificationQueue.append(magnificationValue)
      if magnificationQueue.count == 1 {
        animateQueuedMagnification()
      }
    } else {
      scrollView.magnification = CGFloat(magnificationValue)
    }
  }

  private func animateQueuedMagnification() {
    guard let scrollView = scrollView,
        typefaceView = typefaceView else {
      return
    }
    NSAnimationContext.runAnimationGroup(
        { (_: NSAnimationContext) in
          guard let magnification = self.magnificationQueue.first else {
            return
          }
          scrollView.animator().magnification = CGFloat(magnification)
          self.magnificationQueue.removeFirst()
        },
        completionHandler: { () in
          typefaceView.needsDisplay = true
          if !self.magnificationQueue.isEmpty {
            self.animateQueuedMagnification()
          }
        })
  }

  func zoomToFitAnimated(animated: Bool) {
    guard let scrollView = scrollView,
        documentView = scrollView.documentView else {
      return
    }
    let magnification = min(
        Double(scrollView.bounds.width / documentView.frame.width),
        Double(scrollView.bounds.height / documentView.frame.height))
    setMagnification(magnification, animated: animated)
    alwaysZoomsToFit = true
  }

  @IBAction func zoomToFit(sender: AnyObject?) {
    zoomToFitAnimated(true)
  }

  @IBAction func zoomIn(sender: AnyObject?) {
    guard let scrollView = scrollView else {
      return
    }
    alwaysZoomsToFit = false
    var result = magnificationValue
    var proposed = Double(scrollView.minMagnification)
    for i in 0..<Int.max {
      if proposed >= 1.0 {
        proposed /= (i % 2 != 0) ? 3.0 / 4.0 : 2.0 / 3.0
      } else {
        proposed /= (i % 2 != 0) ? 2.0 / 3.0 : 3.0 / 4.0
      }
      if (magnificationValue >= proposed) {
        continue
      }
      if (result <= magnificationValue && magnificationValue < proposed) {
        result = proposed
        break
      }
      result = proposed
    }
    setMagnification(result, animated: true)
  }

  @IBAction func zoomOut(sender: AnyObject?) {
    guard let scrollView = scrollView else {
      return
    }
    alwaysZoomsToFit = false
    var result = magnificationValue
    var proposed = Double(scrollView.maxMagnification)
    for i in 0..<Int.max {
      if proposed <= 1.0 {
        proposed *= (i % 2 != 0) ? 3.0 / 4.0 : 2.0 / 3.0
      } else {
        proposed *= (i % 2 != 0) ? 2.0 / 3.0 : 3.0 / 4.0
      }
      if magnification <= proposed {
        continue
      }
      if result >= magnification && magnification > proposed {
        result = proposed
        break
      }
      result = proposed
    }
    setMagnification(result, animated: true)
  }
}
