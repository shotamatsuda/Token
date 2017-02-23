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

@objc enum TypefaceUnit : Int {
  case millimeter = 0
  case point = 1
  case inch = 2

  var name: String {
    get {
      switch self {
      case .millimeter:
        return "mm"
      case .point:
        return "pt"
      case .inch:
        return "in"
      }
    }
  }

  func convert(_ value: Double, to: TypefaceUnit) -> Double {
    guard self != to else {
      return value
    }
    switch self {
    case .millimeter:
      switch to {
      case .point:
        return value / (25.4 / 72.0)
      case .inch:
        return value / 25.4
      default:
        assertionFailure()
      }
    case .point:
      switch to {
      case .millimeter:
        return value * (25.4 / 72.0)
      case .inch:
        return value / 72.0
      default:
        assertionFailure()
      }
    case .inch:
      switch to {
      case .millimeter:
        return value * 25.4
      case .point:
        return value * 72.0
      default:
        assertionFailure()
      }
    }
    fatalError()
  }
}
