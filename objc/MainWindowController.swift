//
//  MainWindowController.swift
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

class MainWindowController : NSWindowController, NSWindowDelegate {
  var typeface: TKNTypeface?

  var mainViewController: MainViewController? {
    get {
      return contentViewController as? MainViewController
    }
  }

  var typefaceViewController: TypefaceViewController? {
    get {
      return mainViewController?.typefaceViewController
    }
  }

  var settingsViewController: SettingsViewController? {
    get {
      return mainViewController?.settingsViewController
    }
  }

  override func windowDidLoad() {
    super.windowDidLoad()
    guard let window = window else {
      return
    }
    window.movableByWindowBackground = true
    window.titlebarAppearsTransparent = true
    window.titleVisibility = .Hidden
    window.styleMask |= NSFullSizeContentViewWindowMask

    // Prepare a typeface
    let path = NSBundle.mainBundle().pathForResource("typeface", ofType: nil)
    typeface = TKNTypeface(fileAtPath: path)
    typefaceViewController?.typeface = typeface
    settingsViewController?.typeface = typeface

    // Check for Adobe FDK and show the welcome sheet if necessary.
    dispatch_async(dispatch_get_main_queue()) { () in
      let fileManager = NSFileManager.defaultManager()
      if !fileManager.fileExistsAtPath(FilePath.adobeFDK) {
        self.installAdobeFDK(self)
      }
    }
  }

  // MARK: Actions

  @IBAction func exportFont(sender: AnyObject?) {
    guard let typeface = typeface else {
      return
    }
    let panel = NSSavePanel()
    panel.nameFieldStringValue = typeface.postscriptName + ".otf"
    panel.beginSheetModalForWindow(window!) { (result: Int) in
      if result == NSFileHandlingPanelOKButton {
        typeface.saveToFile(panel.URL!.path!)
      }
    }
  }

  @IBAction func installFont(sender: AnyObject?) {
    guard let typeface = typeface else {
      return
    }
    guard let searchPath = NSSearchPathForDirectoriesInDomains(
      .LibraryDirectory, .UserDomainMask, true).first else {
        fatalError("Could not retrieve user's library directory")
    }
    let fileName = typeface.postscriptName + ".otf"
    let installPath = NSURL(fileURLWithPath: searchPath)
        .URLByAppendingPathComponent(typeface.familyName)
            .URLByAppendingPathComponent(fileName)
    if typeface.saveToFile(installPath.path!) {
      NSWorkspace.sharedWorkspace().openFile(
          installPath.URLByDeletingLastPathComponent!.path!)
    }
  }

  @IBAction func zoomIn(sender: AnyObject?) {
    typefaceViewController?.zoomIn(sender)
  }

  @IBAction func zoomOut(sender: AnyObject?) {
    typefaceViewController?.zoomOut(sender)
  }

  @IBAction func zoomToFit(sender: AnyObject?) {
    typefaceViewController?.zoomToFit(sender)
  }

  // MARK: Adobe FDK

  @IBAction func installAdobeFDK(sender: AnyObject?) {
    guard let window = window else {
      return
    }
    let storyboard = NSStoryboard(name: "Welcome", bundle: nil)
    let windowController = storyboard.instantiateInitialController()
    guard let sheet = (windowController as? NSWindowController)?.window else {
      return
    }
    window.beginSheet(sheet, completionHandler: nil)
  }

  @IBAction func uninstallAdobeFDK(sender: AnyObject?) {
    let alert = NSAlert()
    alert.alertStyle = .WarningAlertStyle
    alert.messageText = NSLocalizedString(
        "Are you sure you want to permanently erase Adobe FDK?",
        comment: "")
    alert.informativeText = NSLocalizedString(
        "You can’t undo this action.",
        comment: "")
    alert.addButtonWithTitle(NSLocalizedString("Uninstall", comment: ""))
    alert.addButtonWithTitle(NSLocalizedString("Cancel", comment: ""))
    let completionHandler = { (returnCode: NSModalResponse) in
      guard returnCode == NSAlertFirstButtonReturn else {
        return
      }
      let applicationSupportPath = FilePath.privateApplicationSupport
      let fileManager = NSFileManager.defaultManager()
      do {
        if fileManager.fileExistsAtPath(applicationSupportPath) {
          try fileManager.removeItemAtPath(applicationSupportPath)
        }
        let libraryPath = FilePath.privateLibrary
        if fileManager.fileExistsAtPath(libraryPath) {
          try fileManager.removeItemAtPath(libraryPath)
        }
      } catch let error {
        let alert = NSAlert()
        alert.messageText = "\(error)"
        alert.beginSheetModalForWindow(
            NSApp.mainWindow!,
            completionHandler: nil)
      }
    }
    alert.beginSheetModalForWindow(
        window!,
        completionHandler: completionHandler)
  }
}
