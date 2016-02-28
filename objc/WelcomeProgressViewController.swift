//
//  WelcomeProgressViewController.swift
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

class WelcomeProgressViewController : NSViewController,
    NSURLSessionDownloadDelegate {
  @IBOutlet var progressIndicator: NSProgressIndicator?
  @IBOutlet var progressLabel: NSTextField?

  override func viewDidAppear() {
    super.viewDidAppear()
    progressIndicator?.startAnimation(self)
    download()
  }

  @IBAction func cancel(sender: AnyObject?) {
    downloadTask?.cancel()
    unzipTask?.terminate()
    guard let window = view.window, sheetParent = window.sheetParent else {
      return
    }
    sheetParent.endSheet(window, returnCode:NSModalResponseCancel)
  }

  // MARK: Downloading

  private var downloadSession: NSURLSession?
  private var downloadTask: NSURLSessionDownloadTask?

  private func download() {
    guard let url = NSURL(
        string: "https://download.macromedia.com" +
                "/pub/developer/opentype/FDK-25-MAC.zip") else {
      return
    }
    downloadSession = NSURLSession(
        configuration: NSURLSessionConfiguration.defaultSessionConfiguration(),
        delegate: self,
        delegateQueue: NSOperationQueue.mainQueue())
    downloadTask = downloadSession!.downloadTaskWithURL(url)
    guard downloadTask != nil else {
      return
    }
    downloadTask!.resume()
    progressIndicator?.indeterminate = false
    progressLabel?.stringValue = NSLocalizedString(
        "Downloading...",
        comment: "")
  }

  func URLSession(
      session: NSURLSession,
      downloadTask: NSURLSessionDownloadTask,
      didWriteData bytesWritten: Int64,
      totalBytesWritten: Int64,
      totalBytesExpectedToWrite: Int64) {
    self.progressIndicator?.doubleValue =
        Double(totalBytesWritten) / Double(totalBytesExpectedToWrite)
  }

  func URLSession(
      session: NSURLSession,
      downloadTask: NSURLSessionDownloadTask,
      didFinishDownloadingToURL location: NSURL) {
    let directory = FilePath.privateApplicationSupport
    let path = NSURL(fileURLWithPath: directory).URLByAppendingPathComponent(
        location.pathComponents!.last!).path!
    let fileManager = NSFileManager.defaultManager()
    do {
      if !fileManager.fileExistsAtPath(directory) {
        try fileManager.createDirectoryAtPath(
            directory,
            withIntermediateDirectories: true,
            attributes: nil)
        try fileManager.copyItemAtPath(location.path!, toPath: path)
      }
      if fileManager.fileExistsAtPath(path) {
        try fileManager.removeItemAtPath(path)
      }
      try fileManager.copyItemAtPath(location.path!, toPath: path)
    } catch let error {
      let alert = NSAlert()
      alert.messageText = "\(error)"
      alert.beginSheetModalForWindow(
          NSApp.mainWindow!,
          completionHandler: nil)
    }
    self.archiveDirectory = directory
    self.archivePath = path
  }

  func URLSession(
      session: NSURLSession,
      task: NSURLSessionTask,
      didCompleteWithError error: NSError?) {
    guard error == nil else {
      let alert = NSAlert(error: error!)
      alert.beginSheetModalForWindow(
          NSApp.mainWindow!,
          completionHandler: nil)
      return
    }
    self.extract()
  }

  // MARK: Extracting

  private var unzipTask: NSTask?
  private var archiveDirectory: String?
  private var archivePath: String?

  private func extract() {
    progressIndicator?.indeterminate = true
    progressIndicator?.startAnimation(self)
    progressLabel?.stringValue = NSLocalizedString(
        "Extracting...",
        comment: "")
    unzipTask = NSTask()
    unzipTask!.launchPath = "/usr/bin/unzip"
    unzipTask!.currentDirectoryPath = archiveDirectory!
    unzipTask!.arguments = ["-qo", archivePath!]
    unzipTask!.terminationHandler = { (task: NSTask) in
      self.didExtract()
    }
    unzipTask!.launch()
  }

  private func didExtract() {
    // This function ill be invoked in a background thread
    dispatch_async(dispatch_get_main_queue()) { () in
      self.install()
      self.cleanUp()
      self.progressIndicator?.stopAnimation(self)
      guard let window = self.view.window,
          sheetParent = window.sheetParent else {
        return
      }
      sheetParent.endSheet(window, returnCode:NSModalResponseOK)
    }
  }

  // MARK: Install

  private func install() {
    progressLabel?.stringValue = NSLocalizedString(
        "Installing...",
        comment: "")
    let libraryPath = FilePath.privateLibrary
    let linkPath = FilePath.adobeFDK
    let path = NSURL(fileURLWithPath: archiveDirectory!)
        .URLByAppendingPathComponent("FDK").path!
    let fileManager = NSFileManager.defaultManager()
    do {
      if !fileManager.fileExistsAtPath(libraryPath) {
        try fileManager.createDirectoryAtPath(
            libraryPath,
            withIntermediateDirectories: true,
            attributes: nil)
      }
      try! fileManager.removeItemAtPath(linkPath)
      try fileManager.createSymbolicLinkAtPath(
          linkPath,
          withDestinationPath: path)
    } catch let error {
      let alert = NSAlert()
      alert.messageText = "\(error)"
      alert.beginSheetModalForWindow(
          NSApp.mainWindow!,
          completionHandler: nil)
    }
  }

  private func cleanUp() {
    progressLabel?.stringValue = NSLocalizedString(
        "Cleaning up...",
        comment: "")
    let garbagePath = archiveDirectory! + "__MACOSX"
    let fileManager = NSFileManager.defaultManager()
    do {
      if fileManager.fileExistsAtPath(garbagePath) {
        try fileManager.removeItemAtPath(garbagePath)
      }
      if fileManager.fileExistsAtPath(archivePath!) {
        try fileManager.removeItemAtPath(archivePath!)
      }
    } catch let error {
      let alert = NSAlert()
      alert.messageText = "\(error)"
      alert.beginSheetModalForWindow(
          NSApp.mainWindow!,
          completionHandler: nil)
    }
  }
}
