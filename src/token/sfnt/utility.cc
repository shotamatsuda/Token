//
//  token/sfnt/utility.cc
//
//  The MIT License
//
//  Copyright (C) 2015-2016 Shota Matsuda
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

#include "token/sfnt/utility.h"

#include <cassert>
#include <fstream>
#include <string>
#include <vector>

#include "sfntly/font.h"
#include "sfntly/font_factory.h"
#include "sfntly/port/file_input_stream.h"
#include "sfntly/port/memory_output_stream.h"
#include "sfntly/tag.h"

namespace token {
namespace sfnt {

namespace {

static void loadFonts(const std::string& path,
                      sfntly::FontFactory *factory,
                      std::vector<sfntly::FontPtr> *fonts) {
  assert(!path.empty());
  assert(factory);
  assert(fonts);
  sfntly::FileInputStream input_stream;
  input_stream.Open(path.c_str());
  factory->LoadFonts(&input_stream, fonts);
  input_stream.Close();
}

static void loadFontBuilders(const std::string& path,
                             sfntly::FontFactory *factory,
                             std::vector<sfntly::FontBuilderPtr> *builders) {
  assert(!path.empty());
  assert(factory);
  assert(builders);
  sfntly::FileInputStream input_stream;
  input_stream.Open(path.c_str());
  factory->LoadFontsForBuilding(&input_stream, builders);
  input_stream.Close();
}

static void serializeFont(const std::string& path,
                          sfntly::FontFactory *factory,
                          sfntly::Font *font) {
  assert(!path.empty());
  assert(factory);
  assert(font);
  sfntly::MemoryOutputStream output_stream;
  factory->SerializeFont(font, &output_stream);
  std::ofstream file;
  file.open(path, std::ios::out | std::ios::binary | std::ios::trunc);
  file.write(reinterpret_cast<const char *>(output_stream.Get()),
             output_stream.Size());
  file.close();
}

}  // namespace

sfntly::Font * loadFont(const std::string& path) {
  assert(!path.empty());
  sfntly::FontFactoryPtr factory;
  factory.Attach(sfntly::FontFactory::GetInstance());
  std::vector<sfntly::FontPtr> fonts;
  loadFonts(path, factory, &fonts);
  assert(!fonts.empty());
  return fonts.front().Detach();
}

sfntly::Font::Builder * loadFontBuilder(const std::string& path) {
  assert(!path.empty());
  sfntly::FontFactoryPtr factory;
  factory.Attach(sfntly::FontFactory::GetInstance());
  std::vector<sfntly::FontBuilderPtr> builders;
  loadFontBuilders(path, factory, &builders);
  assert(!builders.empty());
  return builders.front().Detach();
}

void serializeFont(const std::string& path, sfntly::Font *font) {
  assert(!path.empty());
  assert(font);
  sfntly::FontFactoryPtr factory;
  factory.Attach(sfntly::FontFactory::GetInstance());
  serializeFont(path, factory, font);
}

}  // namespace sfnt
}  // namespace token
