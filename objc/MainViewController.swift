//
//  MainViewController.swift
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

@objc(TKNMainViewController)
class MainViewController : NSViewController {
  private(set) var typefaceViewController: TypefaceViewController?
  private(set) var settingsViewController: SettingsViewController?

  override func prepareForSegue(segue: NSStoryboardSegue, sender: AnyObject?) {
    super.prepareForSegue(segue, sender: sender)
    guard let identifier = segue.identifier else {
      return
    }
    switch identifier {
    case "Typeface":
      typefaceViewController =
          segue.destinationController as? TypefaceViewController
    case "Settings":
      settingsViewController =
          segue.destinationController as? SettingsViewController
    default:
      break
    }
  }

  override func viewDidLoad() {
    super.viewDidLoad()
    typefaceViewController?.view.appearance =
        NSAppearance(named: NSAppearanceNameAqua)
    settingsViewController?.view.appearance =
        NSAppearance(named: NSAppearanceNameVibrantDark)
  }
}
