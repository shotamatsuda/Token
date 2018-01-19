// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

import AppKit

@objc(TKNNumberToPercentageTransformer)
class NumberToPercentageTransformer : ValueTransformer {
  override class func transformedValueClass() -> AnyClass {
    return NSString.self
  }

  override func transformedValue(_ value: Any?) -> Any? {
    guard let value = value as? NSNumber else {
      return nil
    }
    let formatter = NumberFormatter()
    formatter.numberStyle = .percent
    formatter.maximumSignificantDigits = 4
    return formatter.string(from: value)
  }
}
