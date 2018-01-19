// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

import AppKit
import WebKit

class WelcomeAgreementViewController : NSViewController, WebUIDelegate {
  @IBOutlet var licenseWebView: WebView?

  override func viewDidLoad() {
    super.viewDidLoad()
    guard let licenseWebView = licenseWebView else {
      return
    }
    let bundle = Bundle.main
    let url = bundle.url(forResource: "AdobeFDKLicense", withExtension: "html")!
    licenseWebView.mainFrame.load(URLRequest(url: url))
  }

  @IBAction func cancel(_ sender: AnyObject?) {
    guard let window = view.window, let sheetParent = window.sheetParent else {
      return
    }
    sheetParent.endSheet(window, returnCode: .cancel)
  }

  // MARK: WebUIDelegate

  func webView(_ sender: WebView!,
      contextMenuItemsForElement element: [AnyHashable : Any]!,
      defaultMenuItems: [Any]!) -> [Any]! {
    if let menuItem = defaultMenuItems.first as? NSMenuItem {
      if (menuItem.tag == WebMenuItemTagReload) {
        return nil
      }
    }
    return defaultMenuItems
  }
}
