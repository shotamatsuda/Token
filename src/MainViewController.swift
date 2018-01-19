// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

import AppKit

class MainViewController : NSViewController {
  private(set) var typefaceViewController: TypefaceViewController?
  private(set) var settingsViewController: SettingsViewController?

  override func prepare(for segue: NSStoryboardSegue, sender: Any?) {
    super.prepare(for: segue, sender: sender)
    guard let identifier = segue.identifier else {
      return
    }
    switch identifier {
    case NSStoryboardSegue.Identifier("Typeface"):
      typefaceViewController =
          segue.destinationController as? TypefaceViewController
    case NSStoryboardSegue.Identifier("Settings"):
      settingsViewController =
          segue.destinationController as? SettingsViewController
    default:
      break
    }
  }

  override func viewDidLoad() {
    super.viewDidLoad()
    typefaceViewController?.view.appearance = NSAppearance(named: .aqua)
    settingsViewController?.view.appearance = NSAppearance(named: .vibrantDark)
  }
}
