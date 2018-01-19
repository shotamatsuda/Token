// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

import AppKit

class PreferencesViewController : NSViewController {
  @objc var updateCheckInterval: Int {
    get {
      let interval = SUUpdater.shared().updateCheckInterval
      switch interval {
      case 3600.0:
        return 0
      case 86400.0:
        return 1
      case 604800.0:
        return 2
      default:
        break
      }
      return 1
    }

    set(value) {
      switch value {
      case 0:
        SUUpdater.shared().updateCheckInterval = 3600.0;
      case 1:
        SUUpdater.shared().updateCheckInterval = 86400.0;
      case 2:
        SUUpdater.shared().updateCheckInterval = 604800.0;
      default:
        break
      }
    }
  }
}
