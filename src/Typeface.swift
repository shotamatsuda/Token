//
//  Typeface.swift
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

class Typeface : TKNTypeface {
  init(directoryURL: NSURL) {
    _strokeWidth = 0.2
    _capHeight = 2.5
    typographicStroker = TypefaceStroker(
        contentsOfURL: directoryURL.URLByAppendingPathComponent(
            "typographic.ufo"))!
    physicalStroker = TypefaceStroker(
        contentsOfURL: directoryURL.URLByAppendingPathComponent(
            "physical.ufo"))!
    super.init()
    updateStroker()
  }

  // MARK: Stroker

  var behavior: TypefaceBehavior = .Physical

  private var typographicStroker: TypefaceStroker
  private var physicalStroker: TypefaceStroker

  private var stroker: TypefaceStroker {
    get {
      switch behavior {
      case .Physical:
        return physicalStroker
      case .Typographic:
        return typographicStroker
      }
    }
  }

  private func updateStroker() {
    switch behavior {
    case .Physical:
      let strokeWidth = strokeWidthUnit.convert(_strokeWidth, to: .Point)
      let capHeight = capHeightUnit.convert(_capHeight, to: .Point)
      stroker.strokeWidth = (strokeWidth * stroker.capHeight) / capHeight
    case .Typographic:
      stroker.strokeWidth = _strokeWidth
    }
  }

  // MARK: Properties

  var familyName: String {
    get {
      return stroker.familyName
    }
  }

  class func keyPathsForValuesAffectingFamilyName() -> NSSet {
    return NSSet(object: "behavior")
  }

  var styleName: String {
    get {
      switch behavior {
      case .Physical:
        let formatter = NSNumberFormatter()
        formatter.numberStyle = .DecimalStyle
        formatter.minimumFractionDigits = 2
        formatter.maximumFractionDigits = 2
        return formatter.stringFromNumber(strokeWidth)! + strokeWidthUnit.name +
            " / " + formatter.stringFromNumber(capHeight)! + capHeightUnit.name
      case .Typographic:
        return "UPEM " + String(Int(strokeWidth))
      }
    }
  }

  class func keyPathsForValuesAffectingStyleName() -> NSSet {
    return NSSet(objects: "behavior",
        "strokeWidth", "strokeWidthUnit",
        "capHeight", "capHeightUnit")
  }

  var fullName: String {
    get {
      return familyName + " " + styleName
    }
  }

  class func keyPathsForValuesAffectingFullName() -> NSSet {
    return NSSet(objects: "familyName", "styleName")
  }

  var postscriptName: String {
    get {
      switch behavior {
      case .Physical:
        let formatter = NSNumberFormatter()
        formatter.numberStyle = .DecimalStyle
        formatter.minimumFractionDigits = 2
        formatter.maximumFractionDigits = 2
        return familyName + "-" +
            formatter.stringFromNumber(strokeWidth)! + strokeWidthUnit.name +
            "-" + formatter.stringFromNumber(capHeight)! + capHeightUnit.name
      case .Typographic:
        return familyName + "-UPEM-" + String(Int(strokeWidth))
      }
    }
  }

  class func keyPathsForValuesAffectingPostscriptName() -> NSSet {
    return NSSet(objects: "behavior", "familyName",
        "strokeWidth", "strokeWidthUnit",
        "capHeight", "capHeightUnit")
  }

  var ascender: Double {
    get {
      return stroker.ascender
    }
  }

  class func keyPathsForValuesAffectingAscender() -> NSSet {
    return NSSet(object: "behavior")
  }

  var descender: Double {
    get {
      return stroker.descender
    }
  }

  class func keyPathsForValuesAffectingDescender() -> NSSet {
    return NSSet(object: "behavior")
  }

  var lineGap: Double {
    get {
      return stroker.lineGap
    }
  }

  class func keyPathsForValuesAffectingLineGap() -> NSSet {
    return NSSet(object: "behavior")
  }

  // MARK: Stroke Width

  private var _strokeWidth: Double = Double()
  var strokeWidth: Double {
    get {
      return _strokeWidth
    }

    set(value) {
      _strokeWidth = min(max(value, minStrokeWidth), maxStrokeWidth)
      updateStroker()
    }
  }

  var strokeWidthUnit: TypefaceUnit = .Millimeter {
    willSet {
      if behavior != .Physical {
        print("Changing this parameter has no effect " +
              "when the stroker behavior is not physical.")
      }
    }

    didSet {
      if behavior == .Physical && oldValue != strokeWidthUnit {
        strokeWidth = oldValue.convert(strokeWidth, to: strokeWidthUnit)
      }
    }
  }

  var minStrokeWidth: Double {
    get {
      switch behavior {
      case .Physical:
        let other = capHeightUnit.convert(capHeight, to: strokeWidthUnit)
        let coeff = other / stroker.capHeight
        return ceil(coeff * stroker.minStrokeWidth * 100.0) / 100.0
      case .Typographic:
        return stroker.minStrokeWidth
      }
    }
  }

  class func keyPathsForValuesAffectingMinStrokeWidth() -> NSSet {
    return NSSet(object: "behavior")
  }

  var maxStrokeWidth: Double {
    get {
      switch behavior {
      case .Physical:
        let other = capHeightUnit.convert(capHeight, to: strokeWidthUnit)
        let coeff = other / stroker.capHeight
        return floor(coeff * stroker.maxStrokeWidth * 100.0) / 100.0
      case .Typographic:
        return stroker.maxStrokeWidth
      }
    }
  }

  class func keyPathsForValuesAffectingMaxStrokeWidth() -> NSSet {
    return NSSet(object: "behavior")
  }

  private var _capHeight: Double = Double()
  var capHeight: Double {
    get {
      return _capHeight
    }

    set(value) {
      guard behavior == .Physical else {
        print("Changing this parameter has no effect " +
              "when the stroker behavior is not physical.")
        return
      }
      _capHeight = min(max(value, minCapHeight), maxCapHeight)
      updateStroker()
    }
  }

  var capHeightUnit: TypefaceUnit = .Millimeter {
    willSet {
      if behavior != .Physical {
        print("Changing this parameter has no effect " +
              "when the stroker behavior is not physical.")
      }
    }

    didSet {
      if behavior == .Physical && oldValue != capHeightUnit {
        capHeight = oldValue.convert(capHeight, to: capHeightUnit)
      }
    }
  }

  var minCapHeight: Double {
    get {
      switch behavior {
      case .Physical:
        let other = strokeWidthUnit.convert(strokeWidth, to: capHeightUnit)
        let numerator = stroker.capHeight * other
        return ceil(numerator / stroker.maxStrokeWidth * 100.0) / 100.0
      case .Typographic:
        return Double.NaN  // Not used
      }
    }
  }

  class func keyPathsForValuesAffectingMinCapHeight() -> NSSet {
    return NSSet(object: "behavior")
  }

  var maxCapHeight: Double {
    get {
      switch behavior {
      case .Physical:
        let other = strokeWidthUnit.convert(strokeWidth, to: capHeightUnit)
        let numerator = stroker.capHeight * other
        return floor(numerator / stroker.minStrokeWidth * 100.0) / 100.0
      case .Typographic:
        return Double.NaN  // Not used
      }
    }
  }

  class func keyPathsForValuesAffectingMaxCapHeight() -> NSSet {
    return NSSet(object: "behavior")
  }

  // MARK: Glyphs

  func glyphBezierPathForName(name: String) -> NSBezierPath? {
    return stroker.glyphBezierPathForName(name)
  }

  func glyphAdvanceForName(name: String) -> Double {
    return stroker.glyphAdvanceForName(name)
  }

  func glyphBoundsForName(name: String)  -> CGRect {
    return stroker.glyphBoundsForName(name)
  }

  // MARK: Saving

  func createFontToURL(url: NSURL) throws {
    let workingDirectoryURL = NSURL(
        fileURLWithPath: NSTemporaryDirectory()).URLByAppendingPathComponent(
            NSProcessInfo.processInfo().globallyUniqueString)
    let contentsURL = workingDirectoryURL.URLByAppendingPathComponent(
        stroker.url.URLByDeletingPathExtension!
            .URLByAppendingPathExtension("ufo").lastPathComponent!)
    let fontURL = workingDirectoryURL.URLByAppendingPathComponent(
        stroker.url.URLByDeletingPathExtension!
            .URLByAppendingPathExtension("otf").lastPathComponent!)
    let toolsURL = FilePath.adobeFDKURL
        .URLByAppendingPathComponent("Tools")
        .URLByAppendingPathComponent("osx")
    stroker.styleName = styleName
    stroker.fullName = fullName
    stroker.postscriptName = postscriptName
    try stroker.saveToURL(contentsURL)
    try createFontWithContentsOfURL(
        contentsURL,
        toURL: fontURL,
        toolsURL: toolsURL)
    let fileManager = NSFileManager.defaultManager()
    if url.checkResourceIsReachableAndReturnError(nil) {
      try fileManager.removeItemAtURL(url)
    }
    let directoryURL = url.URLByDeletingLastPathComponent!
    if !directoryURL.checkResourceIsReachableAndReturnError(nil) {
      try fileManager.createDirectoryAtURL(
          directoryURL,
          withIntermediateDirectories: true,
          attributes: nil)
    }
    try fileManager.copyItemAtURL(fontURL, toURL: url)
    try fileManager.removeItemAtURL(workingDirectoryURL)
  }
}
