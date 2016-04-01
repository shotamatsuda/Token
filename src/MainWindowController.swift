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
  var typeface: Typeface?

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
    let bundle = NSBundle.mainBundle()
    let URL = bundle.URLForResource("typeface", withExtension: nil)
    typeface = Typeface(directoryURL: URL!)
    restoreTypefaceSettings()
    typefaceViewController?.typeface = typeface
    settingsViewController?.typeface = typeface

    // Check for Adobe FDK and show the welcome sheet if necessary.
    dispatch_async(dispatch_get_main_queue()) { () in
      if !Location.adobeFDKURL.checkResourceIsReachableAndReturnError(nil) {
        self.installAdobeFDK(self)
      }
    }
  }

  func windowWillClose(notification: NSNotification) {
    saveTypefaceSettings()
  }

  // MARK: Settings

  private func saveTypefaceSettings() {
    if let typeface = typeface {
      let defaults = NSUserDefaults.standardUserDefaults()
      defaults.setDouble(
          typeface.strokeWidth,
          forKey: "PhysicalStrokeWidth")
      defaults.setInteger(
          typeface.strokeWidthUnit.rawValue,
          forKey: "PhysicalStrokeWidthUnit")
      defaults.setDouble(
          typeface.capHeight,
          forKey: "PhysicalCapHeight")
      defaults.setInteger(
          typeface.capHeightUnit.rawValue,
          forKey: "PhysicalCapHeightUnit")
      defaults.synchronize()
    }
  }

  private func restoreTypefaceSettings() {
    if let typeface = typeface {
      let defaults = NSUserDefaults.standardUserDefaults()
      if let strokeWidth = defaults.objectForKey("PhysicalStrokeWidth"),
          strokeWidthUnit = defaults.objectForKey("PhysicalStrokeWidthUnit"),
          capHeight = defaults.objectForKey("PhysicalCapHeight"),
          capHeightUnit = defaults.objectForKey("PhysicalCapHeightUnit") {
        typeface.setStrokeWidth(
            strokeWidth as! Double,
            strokeWidthUnit: TypefaceUnit(rawValue: strokeWidthUnit as! Int)!,
            capHeight: capHeight as! Double,
            capHeightUnit: TypefaceUnit(rawValue: capHeightUnit as! Int)!)
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
        do {
          try typeface.createFontToURL(panel.URL!)
        } catch let error as NSError {
          let alert = NSAlert()
          alert.alertStyle = .WarningAlertStyle
          alert.messageText = NSLocalizedString(
              "Couldn’t export font “\(panel.URL!.lastPathComponent!)”.",
              comment: "")
          alert.informativeText = error.localizedDescription
          alert.beginSheetModalForWindow(
              NSApp.mainWindow!,
              completionHandler: nil)
          return
        }
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
    let installURL = NSURL(fileURLWithPath: searchPath)
        .URLByAppendingPathComponent("Fonts")
        .URLByAppendingPathComponent(typeface.familyName)
        .URLByAppendingPathComponent(typeface.postscriptName)
        .URLByAppendingPathExtension("otf")
    do {
      try typeface.createFontToURL(installURL)
    } catch let error as NSError {
      let alert = NSAlert()
      alert.alertStyle = .WarningAlertStyle
      alert.messageText = NSLocalizedString(
          "Couldn’t export font “\(installURL.lastPathComponent!)”.",
          comment: "")
      alert.informativeText = error.localizedDescription
      alert.beginSheetModalForWindow(
          NSApp.mainWindow!,
          completionHandler: nil)
      return
    }
    NSWorkspace.sharedWorkspace().openURL(
        installURL.URLByDeletingLastPathComponent!)
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
      let fileManager = NSFileManager.defaultManager()
      let applicationSupportURL = Location.privateApplicationSupportURL
      let libraryURL = Location.privateLibraryURL
      do {
        if applicationSupportURL.checkResourceIsReachableAndReturnError(nil) {
          try fileManager.removeItemAtURL(applicationSupportURL)
        }
        if libraryURL.checkResourceIsReachableAndReturnError(nil) {
          try fileManager.removeItemAtURL(libraryURL)
        }
      } catch let error as NSError {
        let alert = NSAlert()
        alert.alertStyle = .WarningAlertStyle
        alert.messageText = NSLocalizedString(
            "Couldn’t uninstall Adobe FDK.",
            comment: "")
        alert.informativeText = error.localizedDescription
        alert.beginSheetModalForWindow(
            NSApp.mainWindow!,
            completionHandler: nil)
        return
      }
      let alert = NSAlert()
      alert.messageText = NSLocalizedString(
          "The uninstallation was successful.",
          comment: "")
      alert.informativeText = NSLocalizedString(
          "Adobe FDK was uninstalled.",
          comment: "")
      alert.beginSheetModalForWindow(
          NSApp.mainWindow!,
          completionHandler: nil)
    }
    alert.beginSheetModalForWindow(
        window!,
        completionHandler: completionHandler)
  }
}
