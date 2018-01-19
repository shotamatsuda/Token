// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

struct RectCorners : OptionSet {
  let rawValue: Int

  init(rawValue: Int) {
    self.rawValue = rawValue
  }

  static let topLeft = RectCorners(rawValue: 1 << 0)
  static let bottomLeft = RectCorners(rawValue: 1 << 1)
  static let topRight = RectCorners(rawValue: 1 << 2)
  static let bottomRight = RectCorners(rawValue: 1 << 3)
  static let all: RectCorners = [topLeft, bottomLeft, topRight, bottomRight]
}
