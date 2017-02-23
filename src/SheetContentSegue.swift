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
        [.viewMinXMargin, .viewMaxXMargin, .viewMinYMargin]
    destinationView.autoresizingMask =
        [.viewMinXMargin, .viewMaxXMargin, .viewMinYMargin]

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
      destinationView.autoresizingMask = [.viewWidthSizable, .viewHeightSizable]
    })
    window.setFrame(
        CGRect(origin: window.frame.origin, size: size),
        display: true,
        animate: true)
  }
}
