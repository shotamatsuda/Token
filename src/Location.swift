// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

import AppKit

class Location : NSObject {
  class var privateApplicationSupportURL: URL {
    get {
      guard let identifier = Bundle.main.bundleIdentifier else {
        fatalError("Could not retrieve the main bundle identifier")
      }
      guard let searchPath = NSSearchPathForDirectoriesInDomains(
          .applicationSupportDirectory, .userDomainMask, true).first else {
        fatalError("Could not retrieve user's application support directory")
      }
      return URL(fileURLWithPath: searchPath)
          .appendingPathComponent(identifier)
    }
  }

  class var privateLibraryURL: URL {
    get {
      guard let identifier = Bundle.main.bundleIdentifier else {
        fatalError("Could not retrieve the main bundle identifier")
      }
      guard let searchPath = NSSearchPathForDirectoriesInDomains(
          .libraryDirectory, .userDomainMask, true).first else {
        fatalError("Could not retrieve user's library directory")
      }
      return URL(fileURLWithPath: searchPath)
          .appendingPathComponent("." + identifier)
    }
  }

  class var adobeFDKURL: URL {
    return privateLibraryURL.appendingPathComponent("FDK")
  }
}
