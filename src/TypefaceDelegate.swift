// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

import AppKit

protocol TypefaceDelegate {
  func typeface(_ typeface: Typeface,
      createFontAtURL url: URL,
      didCompleteNumberOfSubtasks numberOfSubtasks: UInt,
      totalNumberOfSubtasks: UInt)

  func typeface(_ typeface: Typeface,
      didFailToCreateFontAtURL url: URL,
      error: NSError)

  func typeface(_ typeface: Typeface, didCreateFontAtURL url: URL)
}

extension TypefaceDelegate {
  func typeface(_ typeface: Typeface,
      createFontAtURL url: URL,
      didCompleteNumberOfSubtasks numberOfSubtasks: UInt,
      totalNumberOfSubtasks: UInt) {}

  func typeface(_ typeface: Typeface,
      didFailToCreateFontAtURL url: URL,
      error: NSError) {}

  func typeface(_ typeface: Typeface, didCreateFontAtURL url: URL) {}
}
