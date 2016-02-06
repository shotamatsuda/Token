//
//  FilePath.swift
//
//  The MIT License
//
//  Copyright (C) 2015 Shota Matsuda
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

@objc(TKNFilePath)
class FilePath : NSObject {
  class var privateApplicationSupport: String {
    get {
      guard let identifier = NSBundle.mainBundle().bundleIdentifier else {
        fatalError("Could not retrieve the main bundle identifier")
      }
      guard let searchPath = NSSearchPathForDirectoriesInDomains(
        .ApplicationSupportDirectory, .UserDomainMask, true).first else {
          fatalError("Could not retrieve user's application support directory")
      }
      let directory = NSURL(fileURLWithPath: searchPath)
      let url = directory.URLByAppendingPathComponent(identifier)
      guard let path = url.path else {
        fatalError("Could not retrieve path of file URL")
      }
      return path
    }
  }

  class var privateLibrary: String {
    get {
      guard let identifier = NSBundle.mainBundle().bundleIdentifier else {
        fatalError("Could not retrieve the main bundle identifier")
      }
      guard let searchPath = NSSearchPathForDirectoriesInDomains(
        .LibraryDirectory, .UserDomainMask, true).first else {
          fatalError("Could not retrieve user's library directory")
      }
      let directory = NSURL(fileURLWithPath: searchPath)
      let url = directory.URLByAppendingPathComponent("." + identifier)
      guard let path = url.path else {
        fatalError("Could not retrieve file path of URL")
      }
      return path
    }
  }

  class var adobeFDK: String {
    let directory = NSURL(fileURLWithPath: privateLibrary)
    let url = directory.URLByAppendingPathComponent("FDK")
    guard let path = url.path else {
      fatalError("Could not retrieve file path of URL")
    }
    return path
  }
}
