// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

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
    switch self {
    case .millimeter:
      switch to {
      case .point:
        return value / (25.4 / 72.0)
      case .inch:
        return value / 25.4
      default:
        return value
      }
    case .point:
      switch to {
      case .millimeter:
        return value * (25.4 / 72.0)
      case .inch:
        return value / 72.0
      default:
        return value
      }
    case .inch:
      switch to {
      case .millimeter:
        return value * 25.4
      case .point:
        return value * 72.0
      default:
        return value
      }
    }
  }
}
