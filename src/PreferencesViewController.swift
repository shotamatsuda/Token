//
//  PreferencesViewController.swift
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

class PreferencesViewController : NSViewController {
  var updateCheckInterval: Int {
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
