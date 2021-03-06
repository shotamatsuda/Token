// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

import AppKit

private var KVOContext: UInt = 0

class TypefaceViewController : NSViewController {
  @IBOutlet var typefaceView: TypefaceView?
  @IBOutlet var scrollView: NSScrollView?

  deinit {
    let defaultCenter = NotificationCenter.default
    defaultCenter.removeObserver(
        self,
        name: NSView.frameDidChangeNotification,
        object: typefaceView)
    defaultCenter.removeObserver(
        self,
        name: NSView.frameDidChangeNotification,
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
    scrollView.autoresizingMask = [.width, .height]
    scrollView.hasHorizontalScroller = true
    scrollView.hasVerticalScroller = true
    scrollView.documentView = typefaceView
    scrollView.allowsMagnification = true
    scrollView.minMagnification = 0.03125
    scrollView.maxMagnification = 64.0

    // Observe for the frames' changes for zooming to fit
    let defaultCenter = NotificationCenter.default
    defaultCenter.addObserver(
        self,
        selector: #selector(contentViewFrameDidChange(_:)),
        name: NSView.frameDidChangeNotification,
        object: typefaceView)
    defaultCenter.addObserver(
        self,
        selector: #selector(contentViewFrameDidChange(_:)),
        name: NSView.frameDidChangeNotification,
        object: scrollView)

    // Inject self to the responder chain
    nextResponder = scrollView.contentView.nextResponder
    scrollView.contentView.nextResponder = self
  }

  override func scrollWheel(with event: NSEvent) {
    guard let scrollView = scrollView else {
      return
    }
    if event.type == .scrollWheel &&
        event.modifierFlags.contains(.option) {
      guard let window = view.window else {
        return
      }
      alwaysZoomsToFit = false
      let center = scrollView.contentView.convert(
          event.locationInWindow,
          from: window.contentView)
      let scale = Double(event.scrollingDeltaY < 0.0 ? 0.9 : 1.0 / 0.9)
      willChangeValue(forKey: "magnification")
      _magnification = min(max(
          _magnification * scale,
          Double(scrollView.minMagnification)),
          Double(scrollView.maxMagnification))
      didChangeValue(forKey: "magnification")
      scrollView.setMagnification(
          CGFloat(_magnification),
          centeredAt: center)
    } else {
      scrollView.scrollWheel(with: event)
    }
  }

  override func magnify(with event: NSEvent) {
    guard let scrollView = scrollView, let window = view.window else {
      return
    }
    alwaysZoomsToFit = false
    let center = scrollView.contentView.convert(
        event.locationInWindow,
        from: window.contentView)
    willChangeValue(forKey: "magnification")
    _magnification = min(max(
        _magnification * (1.0 + Double(event.magnification)),
        Double(scrollView.minMagnification)),
        Double(scrollView.maxMagnification))
    didChangeValue(forKey: "magnification")
    scrollView.setMagnification(
        CGFloat(_magnification),
        centeredAt: center)
  }

  override func observeValue(
      forKeyPath keyPath: String?,
      of object: Any?,
      change: [NSKeyValueChangeKey : Any]?,
      context: UnsafeMutableRawPointer?) {
    if context == &KVOContext {
      if object as? Typeface === typeface {
        typefaceView?.needsDisplay = true
      }
    } else {
      super.observeValue(
          forKeyPath: keyPath,
          of: object,
          change: change,
          context: context)
    }
  }

  @objc func contentViewFrameDidChange(_ notification: Notification) {
    if alwaysZoomsToFit {
      zoomToFitAnimated(false)
    }
  }

  // MARK: Parameters

  private let typefaceKeyPaths = [
      "strokerBehavior", "strokeWidth", "capHeight",
      "strokeWidthUnit", "capHeightUnit"]

  @objc var typeface: Typeface? {
    willSet {
      guard let typeface = typeface else {
        return
      }
      for keyPath in typefaceKeyPaths {
        typeface.removeObserver(self, forKeyPath: keyPath, context: &KVOContext)
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
              options: .new,
              context: &KVOContext)
        }
      }
    }
  }

  @objc var inverted: Bool = false {
    didSet {
      typefaceView?.inverted = inverted
      if inverted {
        scrollView?.backgroundColor = NSColor.black
      } else {
        scrollView?.backgroundColor = NSColor.white
      }
    }
  }

  @objc var outlined: Bool = false {
    didSet {
      typefaceView?.outlined = outlined
    }
  }

  @IBAction func toggleInverted(_ sender: AnyObject?) {
    inverted = !inverted
  }

  @IBAction func toggleOutlined(_ sender: AnyObject?) {
    outlined = !outlined
  }

  // MARK: Zooming

  private var magnificationQueue: Array<Double> = Array<Double>()

  private var _magnification: Double = 1.0
  @objc var magnification: Double {
    get {
      return _magnification
    }

    set(value) {
      setMagnification(value, animated: false)
    }
  }

  @objc var alwaysZoomsToFit: Bool = true {
    didSet {
      if alwaysZoomsToFit != oldValue && alwaysZoomsToFit {
        zoomToFitAnimated(true)
      }
    }
  }

  func setMagnification(_ magnification: Double, animated: Bool) {
    guard let scrollView = scrollView else {
      return
    }
    let value = min(max(
        magnification,
        Double(scrollView.minMagnification)),
        Double(scrollView.maxMagnification))
    if value == _magnification {
      return
    }
    willChangeValue(forKey: "magnification")
    _magnification = value
    didChangeValue(forKey: "magnification")
    if animated {
      magnificationQueue.append(_magnification)
      if magnificationQueue.count == 1 {
        animateQueuedMagnification()
      }
    } else {
      scrollView.magnification = CGFloat(_magnification)
    }
  }

  private func animateQueuedMagnification() {
    guard let scrollView = scrollView,
        let typefaceView = typefaceView else {
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
        completionHandler: {
          typefaceView.needsDisplay = true
          if !self.magnificationQueue.isEmpty {
            self.animateQueuedMagnification()
          }
        })
  }

  func zoomToFitAnimated(_ animated: Bool) {
    guard let scrollView = scrollView,
        let documentView = scrollView.documentView else {
      return
    }
    let magnification = min(
        Double(scrollView.bounds.width / documentView.frame.width),
        Double(scrollView.bounds.height / documentView.frame.height))
    setMagnification(magnification, animated: animated)
    alwaysZoomsToFit = true
  }

  @IBAction func zoomToFit(_ sender: AnyObject?) {
    zoomToFitAnimated(true)
  }

  @IBAction func zoomIn(_ sender: AnyObject?) {
    guard let scrollView = scrollView else {
      return
    }
    alwaysZoomsToFit = false
    var result = _magnification
    var proposed = Double(scrollView.minMagnification)
    for i in 0..<Int.max {
      if proposed >= 1.0 {
        proposed /= (i % 2 != 0) ? 3.0 / 4.0 : 2.0 / 3.0
      } else {
        proposed /= (i % 2 != 0) ? 2.0 / 3.0 : 3.0 / 4.0
      }
      if (_magnification >= proposed) {
        continue
      }
      if (result <= _magnification && _magnification < proposed) {
        result = proposed
        break
      }
      result = proposed
    }
    setMagnification(result, animated: true)
  }

  @IBAction func zoomOut(_ sender: AnyObject?) {
    guard let scrollView = scrollView else {
      return
    }
    alwaysZoomsToFit = false
    var result = _magnification
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
