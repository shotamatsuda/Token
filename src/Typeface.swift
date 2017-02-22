//
//  Typeface.swift
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

class Typeface : TKNTypeface, TypefaceDelegate {
  init(directoryURL: URL) {
    let url = directoryURL.appendingPathComponent("font.ufo")
    _strokeWidth = 0.2
    _capHeight = 2.0
    stroker = Stroker(contentsOf: url)!
    super.init()
    applyPhysicalParameters()
  }

  // MARK: Stroker

  var stroker: Stroker

  private var _strokerBehavior: StrokerBehavior = .physical
  var strokerBehavior: StrokerBehavior {
    get {
      return _strokerBehavior
    }

    set(value) {
      guard value != _strokerBehavior else {
        return
      }
      let strokeWidth = stroker.strokeWidth
      _strokerBehavior = value

      // Maintain parameters so that the stroker's stroke width doesn't change.
      switch strokerBehavior {
      case .default:
        self.strokeWidth = strokeWidth
      case .physical:
        capHeight = physicalCapHeightWithUnit(
            capHeightUnit,
            relativeToStrokeWidth: (self.strokeWidth, strokeWidthUnit),
            forStrokeWidth: strokeWidth,
            usingStroker: stroker)
      }
    }
  }

  private func physicalStrokeWidthWithUnit(
      _ unit: TypefaceUnit,
      relativeToCapHeight relativeTo: (value: Double, unit: TypefaceUnit),
      forStrokeWidth strokeWidth: Double,
      usingStroker stroker: Stroker) -> Double {
    let value = relativeTo.unit.convert(relativeTo.value, to: unit)
    return (value / stroker.capHeight) * strokeWidth
  }

  private func physicalCapHeightWithUnit(
      _ unit: TypefaceUnit,
      relativeToStrokeWidth relativeTo: (value: Double, unit: TypefaceUnit),
      forStrokeWidth strokeWidth: Double,
      usingStroker stroker: Stroker) -> Double {
    let value = relativeTo.unit.convert(relativeTo.value, to: unit)
    return (value * stroker.capHeight) / strokeWidth
  }

  private func applyPhysicalParameters() {
    assert(strokerBehavior == .physical)
    let strokeWidth = strokeWidthUnit.convert(_strokeWidth, to: .point)
    let capHeight = capHeightUnit.convert(_capHeight, to: .point)
    stroker.strokeWidth = (strokeWidth * stroker.capHeight) / capHeight
  }

  // MARK: Properties

  var familyName: String {
    get {
      return stroker.familyName
    }
  }

  class func keyPathsForValuesAffectingFamilyName() -> NSSet {
    return NSSet(object: "strokerBehavior")
  }

  var styleName: String {
    get {
      switch strokerBehavior {
      case .default:
        return "UPEM " + String(Int(strokeWidth))
      case .physical:
        let formatter = NumberFormatter()
        formatter.numberStyle = .decimal
        formatter.minimumFractionDigits = 2
        formatter.maximumFractionDigits = 2
        return formatter.string(from: NSNumber(value: strokeWidth))! + strokeWidthUnit.name +
            " / " + formatter.string(from: NSNumber(value: capHeight))! + capHeightUnit.name
      }
    }
  }

  class func keyPathsForValuesAffectingStyleName() -> NSSet {
    return NSSet(objects: "strokerBehavior",
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
      switch strokerBehavior {
      case .default:
        return familyName + "-UPEM-" + String(Int(strokeWidth))
      case .physical:
        let formatter = NumberFormatter()
        formatter.numberStyle = .decimal
        formatter.minimumFractionDigits = 2
        formatter.maximumFractionDigits = 2
        return familyName + "-" +
            formatter.string(from: NSNumber(value: strokeWidth))! + strokeWidthUnit.name +
            "-" + formatter.string(from: NSNumber(value: capHeight))! + capHeightUnit.name
      }
    }
  }

  class func keyPathsForValuesAffectingPostscriptName() -> NSSet {
    return NSSet(objects: "strokerBehavior", "familyName",
        "strokeWidth", "strokeWidthUnit",
        "capHeight", "capHeightUnit")
  }

  var ascender: Double {
    get {
      return stroker.ascender
    }
  }

  var descender: Double {
    get {
      return stroker.descender
    }
  }

  var lineGap: Double {
    get {
      return stroker.lineGap
    }
  }

  // MARK: Stroke Width

  private var _strokeWidth: Double = Double()
  var strokeWidth: Double {
    get {
      switch strokerBehavior {
      case .default:
        return stroker.strokeWidth
      case .physical:
        return _strokeWidth
      }
    }

    set(value) {
      switch strokerBehavior {
      case .default:
        stroker.strokeWidth = min(max(value, minStrokeWidth), maxStrokeWidth)
      case .physical:
        _strokeWidth = min(max(value, minStrokeWidth), maxStrokeWidth)
        applyPhysicalParameters()
      }
    }
  }

  class func keyPathsForValuesAffectingStrokeWidth() -> NSSet {
    return NSSet(objects: "strokerBehavior", "strokeWidthUnit")
  }

  var strokeWidthUnit: TypefaceUnit = .millimeter {
    willSet {
      if strokerBehavior != .physical {
        print("Changing this parameter has no effect " +
              "when the stroker strokerBehavior is not physical.")
      }
    }

    didSet {
      if strokerBehavior == .physical && oldValue != strokeWidthUnit {
        strokeWidth = oldValue.convert(strokeWidth, to: strokeWidthUnit)
      }
    }
  }

  var minStrokeWidth: Double {
    get {
      switch strokerBehavior {
      case .default:
        return stroker.minStrokeWidth
      case .physical:
        let strokeWidth = physicalStrokeWidthWithUnit(
            strokeWidthUnit,
            relativeToCapHeight: (capHeight, capHeightUnit),
            forStrokeWidth: stroker.minStrokeWidth,
            usingStroker: stroker)
        return ceil(strokeWidth * 100.0) / 100.0
      }
    }
  }

  class func keyPathsForValuesAffectingMinStrokeWidth() -> NSSet {
    return NSSet(objects: "strokerBehavior", "strokeWidthUnit", "capHeight",
                 "capHeightUnit")
  }

  var maxStrokeWidth: Double {
    get {
      switch strokerBehavior {
      case .default:
        return stroker.maxStrokeWidth
      case .physical:
        let strokeWidth = physicalStrokeWidthWithUnit(
            strokeWidthUnit,
            relativeToCapHeight: (capHeight, capHeightUnit),
            forStrokeWidth: stroker.maxStrokeWidth,
            usingStroker: stroker)
        return floor(strokeWidth * 100.0) / 100.0
      }
    }
  }

  class func keyPathsForValuesAffectingMaxStrokeWidth() -> NSSet {
    return NSSet(objects: "strokerBehavior", "strokeWidthUnit", "capHeight",
                 "capHeightUnit")
  }

  private var _capHeight: Double = Double()
  var capHeight: Double {
    get {
      return _capHeight
    }

    set(value) {
      guard strokerBehavior == .physical else {
        print("Changing this parameter has no effect " +
              "when the stroker strokerBehavior is not physical.")
        return
      }
      _capHeight = min(max(value, minCapHeight), maxCapHeight)
      applyPhysicalParameters()
    }
  }

  class func keyPathsForValuesAffectingCapHeight() -> NSSet {
    return NSSet(objects: "strokerBehavior", "capHeightUnit")
  }

  var capHeightUnit: TypefaceUnit = .millimeter {
    willSet {
      if strokerBehavior != .physical {
        print("Changing this parameter has no effect " +
              "when the stroker strokerBehavior is not physical.")
      }
    }

    didSet {
      if strokerBehavior == .physical && oldValue != capHeightUnit {
        capHeight = oldValue.convert(capHeight, to: capHeightUnit)
      }
    }
  }

  var minCapHeight: Double {
    get {
      switch strokerBehavior {
      case .default:
        return Double.nan  // Not used
      case .physical:
        let capHeight = physicalCapHeightWithUnit(
            capHeightUnit,
            relativeToStrokeWidth: (strokeWidth, strokeWidthUnit),
            forStrokeWidth: stroker.maxStrokeWidth,
            usingStroker: stroker)
        return ceil(capHeight * 100.0) / 100.0
      }
    }
  }

  class func keyPathsForValuesAffectingMinCapHeight() -> NSSet {
    return NSSet(objects: "strokerBehavior", "strokeWidth", "strokeWidthUnit",
                 "capHeightUnit")
  }

  var maxCapHeight: Double {
    get {
      switch strokerBehavior {
      case .default:
        return Double.nan  // Not used
      case .physical:
        let capHeight = physicalCapHeightWithUnit(
            capHeightUnit,
            relativeToStrokeWidth: (strokeWidth, strokeWidthUnit),
            forStrokeWidth: stroker.minStrokeWidth,
            usingStroker: stroker)
        return floor(capHeight * 100.0) / 100.0
      }
    }
  }

  class func keyPathsForValuesAffectingMaxCapHeight() -> NSSet {
    return NSSet(objects: "strokerBehavior", "strokeWidth", "strokeWidthUnit",
                 "capHeightUnit")
  }

  func setStrokeWidth(_ strokeWidth: Double,
                      strokeWidthUnit: TypefaceUnit,
                      capHeight: Double,
                      capHeightUnit: TypefaceUnit) {
    self.strokeWidthUnit = strokeWidthUnit
    willChangeValue(forKey: "strokeWidth")
    _strokeWidth = strokeWidth
    didChangeValue(forKey: "strokeWidth")
    self.capHeightUnit = capHeightUnit
    self.capHeight = capHeight
  }

  func setCapHeight(_ capHeight: Double,
                    capHeightUnit: TypefaceUnit,
                    strokeWidth: Double,
                    strokeWidthUnit: TypefaceUnit) {
    self.capHeightUnit = capHeightUnit
    willChangeValue(forKey: "capHeight")
    _capHeight = capHeight
    didChangeValue(forKey: "capHeight")
    self.strokeWidthUnit = strokeWidthUnit
    self.strokeWidth = strokeWidth
  }

  // MARK: Glyphs

  func glyphBezierPathForName(_ name: String) -> NSBezierPath? {
    return stroker.glyphBezierPath(forName: name)
  }

  func glyphAdvanceForName(_ name: String) -> Double {
    return stroker.glyphAdvance(forName: name)
  }

  func glyphBoundsForName(_ name: String) -> CGRect {
    return stroker.glyphBounds(forName: name)
  }

  // MARK: Saving

  var delegate: TypefaceDelegate?

  func createFontToURL(_ url: URL, completionHandler: (() -> Void)?) {
    let stroker = self.stroker.copy() as! Stroker
    let workingDirectoryURL = URL(
        fileURLWithPath: NSTemporaryDirectory()).appendingPathComponent(
            ProcessInfo.processInfo.globallyUniqueString)
    let contentsURL = workingDirectoryURL.appendingPathComponent(
        stroker.url.deletingPathExtension()
            .appendingPathExtension("ufo").lastPathComponent)
    let fontURL = workingDirectoryURL.appendingPathComponent(
        stroker.url.deletingPathExtension()
            .appendingPathExtension("otf").lastPathComponent)
    let toolsURL = Location.adobeFDKURL
        .appendingPathComponent("Tools")
        .appendingPathComponent("osx")
    let extraURL = Bundle.main.url(
        forResource: "Scripts/fdk-extra",
        withExtension: nil)!
    stroker.styleName = styleName
    stroker.fullName = fullName
    stroker.postscriptName = postscriptName
    if strokerBehavior == .physical {
      stroker.upem = stroker.capHeight
    }
    do {
      try stroker.save(to: contentsURL)
    } catch let error as NSError {
      (delegate ?? self).typeface(
          self,
          didFailToCreateFontAtURL: fontURL,
          error: error)
      return
    }
    createFont(
        withContentsOf: contentsURL,
        to: fontURL,
        toolsURL: toolsURL,
        extraURL: extraURL,
        progressHandler: { (numberOfSubtasks: UInt,
                            totalNumberOfSubtasks: UInt) in
          (self.delegate ?? self).typeface(
              self,
              createFontAtURL: fontURL,
              didCompleteNumberOfSubtasks: numberOfSubtasks,
              totalNumberOfSubtasks: totalNumberOfSubtasks)
        },
        completionHandler: {
          self.correctUPEM(
              stroker.upem,
              forFontAt: fontURL,
              toolsURL: toolsURL)
          do {
            let fileManager = FileManager.default
            if (try? url.checkResourceIsReachable()) ?? false {
              try fileManager.removeItem(at: url)
            }
            let directoryURL = url.deletingLastPathComponent()
            if !((try? directoryURL.checkResourceIsReachable()) ?? false) {
              try fileManager.createDirectory(
                  at: directoryURL,
                  withIntermediateDirectories: true,
                  attributes: nil)
            }
            try fileManager.copyItem(at: fontURL, to: url)
            try fileManager.removeItem(at: workingDirectoryURL)
          } catch let error as NSError {
            (self.delegate ?? self).typeface(
                self,
                didFailToCreateFontAtURL: fontURL,
                error: error)
            return
          }
          completionHandler?()
        })
  }
}
