// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

import AppKit

@objcMembers
class Stroker : TKNStroker {
  static private let MinStrokeWidth: Double = 10.0
  static private let MaxStrokeWidth: Double = 110.0

  var minStrokeWidth: Double {
    get {
      return type(of: self).MinStrokeWidth
    }
  }

  var maxStrokeWidth: Double {
    get {
      return floor(type(of: self).MaxStrokeWidth * upem / 1000.0)
    }
  }
}
