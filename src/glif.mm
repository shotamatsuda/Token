//
//  main.cc
//
//  The MIT License
//
//  Copyright (C) 2015 Shota Matsuda
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

#include <Foundation/Foundation.h>

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <streambuf>
#include <string>

#include <boost/filesystem.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "token/glyph_outline.h"
#include "token/glyph_stroker.h"
#include "token/ufo/glyphs.h"
#include "token/ufo/glif.h"

int main(int argc, char **argv) {
  const std::string source = "/Users/sgss/Dropbox/Github/token/Token.ufo";
  const std::string destination = "/Users/sgss/Desktop/Token.ufo";
  @autoreleasepool {
    NSString *from = [NSString stringWithUTF8String:source.c_str()];
    NSString *to = [NSString stringWithUTF8String:destination.c_str()];
    NSFileManager *manager = [NSFileManager defaultManager];
    NSError *error;
    if ([manager fileExistsAtPath:to]) {
      [manager removeItemAtPath:to error:&error];
    }
    [manager copyItemAtPath:from toPath:to error:&error];
  }
  token::ufo::Glyphs glyphs(destination);
  token::GlyphStroker stroker;
  stroker.set_width(4.0);
  for (auto& glyph : glyphs) {
    token::GlyphOutline outline(glyph);
    takram::Shape2d shape;
    shape = stroker.stroke(outline);
    for (auto& command : shape) {
      for (auto& other : shape) {
        if (&command == &other) {
          continue;
        }
        if (std::abs(command.point().x - other.point().x) < 0.01 &&
            std::abs(command.point().y - other.point().y) < 0.01) {
          const auto mid = (command.point() + other.point()) / 2;
          command.point() = mid;
          other.point() = mid;
        }
      }
    }
    shape = stroker.simplify(shape);
    shape.convertConicsToQuadratics();
    shape.convertQuadraticsToCubics();
    shape.removeDuplicates(1.0);
    token::GlyphOutline stroked(glyph, shape);
    glyphs.set(glyph.name, stroked.glyph());
  }
  std::system(("FDK_EXE=\"/Users/sgss/bin/FDK/Tools/osx\";"
               "PATH=${PATH}:\"/Users/sgss/bin/FDK/Tools/osx\";"
               "export PATH;"
               "export FDK_EXE;"
               "makeotf -f \"" + destination + "\"").c_str());
  return EXIT_SUCCESS;
}
