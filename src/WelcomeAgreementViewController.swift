//
//  WelcomeAgreementViewController.swift
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

class WelcomeAgreementViewController : NSViewController {
  @IBOutlet var licenseTextView: NSTextView?

  override func viewDidLoad() {
    super.viewDidLoad()
    guard let licenseTextView = licenseTextView else {
      return
    }
    let bundle = NSBundle.mainBundle()
    let URL = bundle.URLForResource("AdobeFDKLicense", withExtension: "rtf")!
    let data = NSData(contentsOfURL: URL)!
    let contents = NSAttributedString(RTF: data, documentAttributes: nil)
    licenseTextView.textStorage!.setAttributedString(contents!)
  }

  @IBAction func cancel(sender: AnyObject?) {
    guard let window = view.window, sheetParent = window.sheetParent else {
      return
    }
    sheetParent.endSheet(window, returnCode:NSModalResponseCancel)
  }
}