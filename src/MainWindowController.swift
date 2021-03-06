// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

import AppKit

class MainWindowController : NSWindowController, NSWindowDelegate,
    TypefaceDelegate {
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

  private var progressViewController: ProgressViewController?
  private var welcomeWindowController: WelcomeWindowController?

  override func windowDidLoad() {
    super.windowDidLoad()
    guard let window = window else {
      return
    }
    window.isMovableByWindowBackground = true
    window.titlebarAppearsTransparent = true
    window.titleVisibility = .hidden
    window.styleMask.insert(.fullSizeContentView)

    // Prepare a typeface
    let bundle = Bundle.main
    let url = bundle.url(forResource: "typeface", withExtension: nil)
    typeface = Typeface(directoryURL: url!)
    typeface!.delegate = self
    restoreTypefaceSettings()
    typefaceViewController?.typeface = typeface
    settingsViewController?.typeface = typeface

    // Check for Adobe FDK and show the welcome sheet if necessary.
    DispatchQueue.main.async {
      if !((try? Location.adobeFDKURL.checkResourceIsReachable()) ?? false) {
        self.installAdobeFDK(self)
      }
    }
  }

  func windowWillClose(_ notification: Notification) {
    saveTypefaceSettings()
  }

  override func validateMenuItem(_ menuItem: NSMenuItem) -> Bool {
    if menuItem.action == #selector(installAdobeFDK(_:)) ||
       menuItem.action == #selector(uninstallAdobeFDK(_:)) {
      return welcomeWindowController == nil
    }
    return true
  }

  // MARK: Settings

  private func saveTypefaceSettings() {
    if let typeface = typeface {
      let defaults = UserDefaults.standard
      defaults.set(
          typeface.strokeWidth,
          forKey: "PhysicalStrokeWidth")
      defaults.set(
          typeface.strokeWidthUnit.rawValue,
          forKey: "PhysicalStrokeWidthUnit")
      defaults.set(
          typeface.capHeight,
          forKey: "PhysicalCapHeight")
      defaults.set(
          typeface.capHeightUnit.rawValue,
          forKey: "PhysicalCapHeightUnit")
      defaults.synchronize()
    }
  }

  private func restoreTypefaceSettings() {
    if let typeface = typeface {
      let defaults = UserDefaults.standard
      if let strokeWidth = defaults.object(forKey: "PhysicalStrokeWidth"),
          let strokeWidthUnit = defaults.object(forKey: "PhysicalStrokeWidthUnit"),
          let capHeight = defaults.object(forKey: "PhysicalCapHeight"),
          let capHeightUnit = defaults.object(forKey: "PhysicalCapHeightUnit") {
        typeface.setStrokeWidth(
            strokeWidth as! Double,
            strokeWidthUnit: TypefaceUnit(rawValue: strokeWidthUnit as! Int)!,
            capHeight: capHeight as! Double,
            capHeightUnit: TypefaceUnit(rawValue: capHeightUnit as! Int)!)
      }
    }
  }

  // MARK: Actions

  private func createFontAtURL(
      _ url: URL,
      message: String,
      completionHandler: (() -> Void)?) {
    progressViewController = storyboard!.instantiateController(
        withIdentifier: NSStoryboard.SceneIdentifier("ProgressViewController"))
            as? ProgressViewController
    contentViewController!.presentViewControllerAsSheet(progressViewController!)
    progressViewController!.progressLabel!.stringValue = message
    typeface!.createFontToURL(url) {
      self.contentViewController!.dismissViewController(
          self.progressViewController!)
      self.progressViewController = nil
      completionHandler?()
    }
  }

  @IBAction func exportFont(_ sender: AnyObject?) {
    if !((try? Location.adobeFDKURL.checkResourceIsReachable()) ?? false) {
      self.installAdobeFDK(self)
      return
    }
    guard let typeface = typeface else {
      return
    }
    let panel = NSSavePanel()
    panel.nameFieldStringValue = typeface.postscriptName + ".otf"
    panel.beginSheetModal(for: window!) { (result) in
      if result == .OK {
        self.createFontAtURL(
            panel.url!,
            message: String(
                format: NSLocalizedString("Exporting “%@”...", comment: ""),
                panel.url!.lastPathComponent),
            completionHandler: nil)
      }
    }
  }

  @IBAction func installFont(_ sender: AnyObject?) {
    if !((try? Location.adobeFDKURL.checkResourceIsReachable()) ?? false) {
      self.installAdobeFDK(self)
      return
    }
    guard let typeface = typeface else {
      return
    }
    guard let searchPath = NSSearchPathForDirectoriesInDomains(
        .libraryDirectory, .userDomainMask, true).first else {
      fatalError("Could not retrieve user's library directory")
    }
    let installURL = URL(fileURLWithPath: searchPath)
        .appendingPathComponent("Fonts")
        .appendingPathComponent(typeface.familyName)
        .appendingPathComponent(typeface.postscriptName)
        .appendingPathExtension("otf")
    self.createFontAtURL(
        installURL,
        message: String(
            format: NSLocalizedString("Installing “%@”...", comment: ""),
            installURL.lastPathComponent)) {
      NSWorkspace.shared.open(
          installURL.deletingLastPathComponent())
    }
  }

  @IBAction func zoomIn(_ sender: AnyObject?) {
    typefaceViewController?.zoomIn(sender)
  }

  @IBAction func zoomOut(_ sender: AnyObject?) {
    typefaceViewController?.zoomOut(sender)
  }

  @IBAction func zoomToFit(_ sender: AnyObject?) {
    typefaceViewController?.zoomToFit(sender)
  }

  // MARK: Typeface Delegate

  func typeface(
      _ typeface: Typeface,
      createFontAtURL url: URL,
      didCompleteNumberOfSubtasks numberOfSubtasks: UInt,
      totalNumberOfSubtasks: UInt) {
    progressViewController!.progressIndicator!.isIndeterminate = false
    progressViewController!.progressIndicator!.doubleValue =
        Double(numberOfSubtasks) / Double(totalNumberOfSubtasks)
  }

  func typeface(
      _ typeface: Typeface,
      didFailToCreateFontAtURL url: URL,
      error: NSError) {
    let alert = NSAlert()
    alert.alertStyle = .warning
    alert.messageText = NSLocalizedString(
        "Couldn’t export font “\(url.lastPathComponent)”.",
        comment: "")
    alert.informativeText = error.localizedDescription
    alert.beginSheetModal(
        for: NSApp.mainWindow!,
        completionHandler: nil)
  }

  // MARK: Adobe FDK

  @IBAction func installAdobeFDK(_ sender: AnyObject?) {
    guard let window = window else {
      return
    }
    if welcomeWindowController == nil {
      let storyboard = NSStoryboard(
          name: NSStoryboard.Name("Welcome"), 
          bundle: nil)
      let controller = storyboard.instantiateInitialController()
      welcomeWindowController = controller as! WelcomeWindowController?
      NotificationCenter.default.addObserver(
          forName: WelcomeWindowController.DidFinishNotification,
          object: welcomeWindowController,
          queue: OperationQueue.main,
          using: { (notification) in
            self.welcomeWindowController = nil
          })
    }
    guard let sheet = welcomeWindowController!.window else {
      return
    }
    window.beginSheet(sheet) { (response) in
      if response != .continue {
        self.welcomeWindowController = nil
      }
    }
  }

  @IBAction func uninstallAdobeFDK(_ sender: AnyObject?) {
    let alert = NSAlert()
    alert.alertStyle = .warning
    alert.messageText = NSLocalizedString(
        "Are you sure you want to permanently erase Adobe FDK?",
        comment: "")
    alert.informativeText = NSLocalizedString(
        "You can’t undo this action.",
        comment: "")
    alert.addButton(withTitle: NSLocalizedString("Uninstall", comment: ""))
    alert.addButton(withTitle: NSLocalizedString("Cancel", comment: ""))
    let completionHandler = { (returnCode: NSApplication.ModalResponse) in
      guard returnCode == .alertFirstButtonReturn else {
        return
      }
      let fileManager = FileManager.default
      let applicationSupportURL = Location.privateApplicationSupportURL
      let libraryURL = Location.privateLibraryURL
      do {
        if (try? applicationSupportURL.checkResourceIsReachable()) ?? false {
          try fileManager.removeItem(at: applicationSupportURL)
        }
        if (try? libraryURL.checkResourceIsReachable()) ?? false {
          try fileManager.removeItem(at: libraryURL)
        }
      } catch let error as NSError {
        let alert = NSAlert()
        alert.alertStyle = .warning
        alert.messageText = NSLocalizedString(
            "Couldn’t uninstall Adobe FDK.",
            comment: "")
        alert.informativeText = error.localizedDescription
        alert.beginSheetModal(
            for: NSApp.mainWindow!,
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
      alert.beginSheetModal(
          for: NSApp.mainWindow!,
          completionHandler: nil)
    }
    alert.beginSheetModal(
        for: window!,
        completionHandler: completionHandler)
  }
}
