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

class WelcomeProgressViewController : NSViewController,
    URLSessionDownloadDelegate {
  @IBOutlet var progressIndicator: NSProgressIndicator?
  @IBOutlet var progressLabel: NSTextField?

  override func viewDidLoad() {
    super.viewDidLoad()
    progressIndicator?.startAnimation(self)
    download()
  }

  @IBAction func background(_ sender: AnyObject?) {
    guard let window = view.window, let sheetParent = window.sheetParent else {
      return
    }
    sheetParent.endSheet(window, returnCode: .continue)
  }

  @IBAction func cancel(_ sender: AnyObject?) {
    downloadTask?.cancel()
    unzipTask?.terminate()
    guard let window = view.window, let sheetParent = window.sheetParent else {
      return
    }
    sheetParent.endSheet(window, returnCode: .cancel)
  }

  // MARK: Downloading

  private var downloadSession: URLSession?
  private var downloadTask: URLSessionDownloadTask?

  private func download() {
    guard let url = URL(
        string: "https://github.com/adobe-type-tools/afdko/releases/download/" +
                "2.5.65322/FDK-25-MAC.b65322.zip") else {
      return
    }
    downloadSession = URLSession(
        configuration: URLSessionConfiguration.default,
        delegate: self,
        delegateQueue: OperationQueue.main)
    downloadTask = downloadSession!.downloadTask(with: url)
    guard downloadTask != nil else {
      return
    }
    downloadTask!.resume()
    progressIndicator?.isIndeterminate = false
    progressLabel?.stringValue = NSLocalizedString(
        "Downloading...",
        comment: "")
  }

  func urlSession(
      _ session: URLSession,
      downloadTask: URLSessionDownloadTask,
      didWriteData bytesWritten: Int64,
      totalBytesWritten: Int64,
      totalBytesExpectedToWrite: Int64) {
    progressIndicator?.doubleValue =
        Double(totalBytesWritten) / Double(totalBytesExpectedToWrite)
  }

  func urlSession(
      _ session: URLSession,
      downloadTask: URLSessionDownloadTask,
      didFinishDownloadingTo location: URL) {
    let directoryURL = Location.privateApplicationSupportURL
    let url = directoryURL.appendingPathComponent(
        downloadTask.originalRequest!.url!.lastPathComponent)
    let fileManager = FileManager.default
    do {
      if !((try? directoryURL.checkResourceIsReachable()) ?? false)  {
        try fileManager.createDirectory(
            at: directoryURL,
            withIntermediateDirectories: true,
            attributes: nil)
        try fileManager.copyItem(at: location, to: url)
      }
      if (try? url.checkResourceIsReachable()) ?? false {
        try fileManager.removeItem(at: url)
      }
      try fileManager.copyItem(at: location, to: url)
    } catch let error as NSError {
      let alert = NSAlert()
      alert.alertStyle = .warning
      alert.messageText = NSLocalizedString(
          "Couldn’t finish downloading Adobe FDK.",
          comment: "")
      alert.informativeText = error.localizedDescription
      alert.beginSheetModal(
          for: NSApp.mainWindow!,
          completionHandler: nil)
    }
    archiveDirectoryURL = directoryURL
    archiveURL = url
  }

  func urlSession(
      _ session: URLSession,
      task: URLSessionTask,
      didCompleteWithError error: Error?) {
    if let error = error as NSError? {
      if error.domain == NSURLErrorDomain &&
         error.code == NSURLErrorCancelled {
        return  // User cancelled
      }
      let alert = NSAlert()
      alert.alertStyle = .warning
      alert.messageText = NSLocalizedString(
          "Couldn’t download Adobe FDK.",
          comment: "")
      alert.informativeText = error.localizedDescription
      alert.beginSheetModal(
          for: NSApp.mainWindow!,
          completionHandler: nil)
      return
    }
    extract()
  }

  // MARK: Extracting

  private var unzipTask: Process?
  private var archiveDirectoryURL: URL?
  private var archiveURL: URL?

  private func extract() {
    progressIndicator?.isIndeterminate = true
    progressIndicator?.startAnimation(self)
    progressLabel?.stringValue = NSLocalizedString(
        "Extracting...",
        comment: "")
    unzipTask = Process()
    unzipTask!.launchPath = "/usr/bin/unzip"
    unzipTask!.currentDirectoryPath = archiveDirectoryURL!.path
    unzipTask!.arguments = ["-qo", archiveURL!.path]
    unzipTask!.terminationHandler = { (task: Process) in
      self.didExtract()
    }
    unzipTask!.launch()
  }

  private func didExtract() {
    // This function ill be invoked in a background thread
    DispatchQueue.main.async {
      defer {
        self.progressIndicator?.stopAnimation(self)
      }
      do {
        try self.install()
        try self.cleanUp()
      } catch let error as NSError {
        let alert = NSAlert()
        alert.alertStyle = .warning
        alert.messageText = NSLocalizedString(
            "Couldn’t install Adobe FDK.",
            comment: "")
        alert.informativeText = error.localizedDescription
        alert.beginSheetModal(
            for: NSApp.mainWindow!,
            completionHandler: nil)
      }
      guard let window = self.view.window else {
        return
      }
      guard let sheetParent = window.sheetParent else {
        NotificationCenter.default.post(
            name: WelcomeWindowController.DidFinishNotification,
            object: window.windowController)
        return
      }
      sheetParent.endSheet(window, returnCode: .OK)
    }
  }

  // MARK: Install

  private func install() throws {
    progressLabel?.stringValue = NSLocalizedString(
        "Installing...",
        comment: "")
    let libraryURL = Location.privateLibraryURL
    let linkURL = Location.adobeFDKURL
    let url = archiveURL!.deletingPathExtension()
    let fileManager = FileManager.default
    if !((try? libraryURL.checkResourceIsReachable()) ?? false) {
      try fileManager.createDirectory(
          at: libraryURL,
          withIntermediateDirectories: true,
          attributes: nil)
    }
    if (try? linkURL.checkResourceIsReachable()) ?? false {
      try fileManager.removeItem(at: linkURL)
    }
    try fileManager.createSymbolicLink(
        at: linkURL,
        withDestinationURL: url)
  }

  private func cleanUp() throws {
    progressLabel?.stringValue = NSLocalizedString(
        "Cleaning up...",
        comment: "")
    let garbageURL = archiveDirectoryURL!
        .appendingPathComponent("__MACOSX")
    let fileManager = FileManager.default
    if (try? garbageURL.checkResourceIsReachable()) ?? false {
      try fileManager.removeItem(at: garbageURL)
    }
    if (try? archiveURL!.checkResourceIsReachable()) ?? false {
      try fileManager.removeItem(at: archiveURL!)
    }
  }
}
