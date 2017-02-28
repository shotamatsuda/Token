Token
=====

Token is an app to generate OpenType fonts of a product typeface designed for [TAMRON New SP Series](https://takram.com/projects/tamron/), applying arbitrary weight by defining cap height and stroke width in physical length.

[![License](http://img.shields.io/badge/license-MIT-lightgrey.svg?style=flat
)](http://mit-license.org)

![](https://cloud.githubusercontent.com/assets/8651513/23328503/c8e1efd0-fb65-11e6-9ba7-91f5aa58d491.jpg)

![](https://cloud.githubusercontent.com/assets/8651513/23328504/c9e5c44c-fb65-11e6-9604-ec3e7adb79fe.png)

## Typeface Definition

Font information, glyph shapes, metrics as well as kerning are stored as [Unified Font Object](http://unifiedfontobject.org) in [`typeface/font.ufo`](typeface/font.ufo). It uses 2000 units per em for a higher level of precision to scale kerning values, which are always rounded to integers, whereas AFDKO retains decimal coordinates in glyph outlines.

Each [GLIF](http://unifiedfontobject.org/versions/ufo3/glyphs/glif/) file may contain some conventional data in the custom data storage section in order to control Token’s path stroking and set operations. `numberOfContours` and `numberOfHoles` take an integer value that hints to Token’s stroker the number of contours or holes in a glyph after being stroked. `contourStyles` is a dictionary of contour identifiers and stroke styles described in the table below.

Attribute | Type | Description | Default
--- | --- | --- | ---
`cap` | string | Specifies the shape to be used at the start and end of a contour. Possible values are: `butt`, `round` and `project`. | `round`
`join` | string | Specifies the shape to be used at the corners of a contour. Possible values are: `miter`, `round` and `bevel`. | `round`
`align` | string | Specifies if the position of a contour moves by half a stroke width to right so that the stroked path aligns to the boundary of the original path at the left (`left`) or right (`right`). | `none`
`filled` | boolean | Determines if a contour is filled (`true`) or stroked (`false`). | `false`

## Stroking

Stroking a Bézier curve is not as easy as it looks. Since a curve parallel to a Bézier curve cannot be represented as a Bézier curve, it needs a heuristic approach that gives a practical approximation. Among several algorithms and libraries such as [Fast, precise flattening of cubic Bézier path and offset curves](https://pdfs.semanticscholar.org/9679/e1295261d77cd9ef514b9c875953fe1d2c80.pdf), [FreeType Glyph Stroker](https://www.freetype.org/freetype2/docs/reference/ft2-glyph_stroker.html) or [QPainterPathStroker](http://doc.qt.io/qt-5/qpainterpathstroker.html), Token uses [Skia](https://skia.org)’s path stroker and set operations considering robustness and precision.

## Compiling

Install the following dependencies via [Homebrew](https://brew.sh),

- boost (developed on 1.63.0)
- libplist (developed on 1.12)

and run [`skia.sh`](script/skia.sh) inside the [`script`](script) folder in this repository to download and build [Skia](https://skia.org) (this will take a time to complete since it's a large piece of source codes). After making sure to init and update the submodules, you should be able to open the project file in Xcode and build it.

## License

The MIT License

Copyright (C) 2015-2017 Shota Matsuda

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.

## Credits

- Design & Development: Shota Matsuda (Takram)
- Typeface Design: Ken Okamoto
