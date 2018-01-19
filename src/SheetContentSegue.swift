// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

import AppKit

class SheetContentSegue : NSStoryboardSegue {
  override func perform() {
    guard let sourceController = sourceController as? NSViewController,
        let destinationController = destinationController as? NSViewController,
        let containerController = sourceController.parent,
        let window = containerController.view.window else {
      return
    }
    let sourceView = sourceController.view
    let destinationView = destinationController.view
    let containerView = containerController.view
    containerController.addChildViewController(destinationController)
    containerView.addSubview(destinationView)

    // Temporarily disable auto layout
    sourceView.autoresizingMask =
        [.minXMargin, .maxXMargin, .minYMargin]
    destinationView.autoresizingMask =
        [.minXMargin, .maxXMargin, .minYMargin]

    // Horizontally centerize the destination view
    let size = destinationView.frame.size
    destinationView.frame = CGRect(
        x: (window.frame.width - size.width) / 2.0,
        y: window.frame.height - size.height,
        width: size.width,
        height: size.height)

    // Prepare for animation
    sourceView.wantsLayer = true
    destinationView.wantsLayer = true
    destinationView.alphaValue = 0.0

    // Resize views and window simultaneously
    NSAnimationContext.runAnimationGroup({ (context: NSAnimationContext) in
      sourceView.animator().alphaValue = 0.0
      destinationView.animator().alphaValue = 1.0
    }, completionHandler: {
      sourceView.removeFromSuperview()
      sourceController.removeFromParentViewController()
      destinationView.autoresizingMask = [.width, .height]
    })
    window.setFrame(
        CGRect(origin: window.frame.origin, size: size),
        display: true,
        animate: true)
  }
}
