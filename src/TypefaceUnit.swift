////
////  TypefaceUnit.swift
////
////  The MIT License
////
////  Copyright (C) 2015-2016 Shota Matsuda
////
////  Permission is hereby granted, free of charge, to any person obtaining a
////  copy of this software and associated documentation files (the "Software"),
////  to deal in the Software without restriction, including without limitation
////  the rights to use, copy, modify, merge, publish, distribute, sublicense,
////  and/or sell copies of the Software, and to permit persons to whom the
////  Software is furnished to do so, subject to the following conditions:
////
////  The above copyright notice and this permission notice shall be included in
////  all copies or substantial portions of the Software.
////
////  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
////  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
////  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
////  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
////  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
////  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
////  DEALINGS IN THE SOFTWARE.
////

import AppKit

class TypefaceUnit : NSObject {
  enum Type {
    case Millimeter
    case Point
    case Inch
  }

  let rawValue: Type

  private override init() {
    rawValue = .Millimeter
    super.init()
  }

  private init(rawValue: Type) {
    self.rawValue = rawValue
  }

  var name: String {
    get {
      switch (rawValue) {
      case .Millimeter:
        return "mm"
      case .Point:
        return "pt"
      case .Inch:
        return "in"
      }
    }
  }

  class func convert(
      value: Double,
      from: TypefaceUnit,
      to: TypefaceUnit) -> Double {
    guard from.rawValue == to.rawValue else {
      return value
    }
    switch (from.rawValue) {
    case .Millimeter:
      switch (to.rawValue) {
      case .Point:
        return value / (25.4 / 72.0)
      case .Inch:
        return value / 25.4
      default:
        fatalError()
      }
    case .Point:
      switch (to.rawValue) {
      case .Millimeter:
        return value * (25.4 / 72.0)
      case .Inch:
        return value / 72.0
      default:
        fatalError()
      }
    case .Inch:
      switch (to.rawValue) {
      case .Millimeter:
        return value * 25.4
      case .Point:
        return value * 72.0
      default:
        fatalError()
      }
    }
    fatalError()
  }

  static let Millimeter = TypefaceUnit(rawValue: .Millimeter)
  static let Point = TypefaceUnit(rawValue: .Point)
  static let Inch = TypefaceUnit(rawValue: .Inch)
}
