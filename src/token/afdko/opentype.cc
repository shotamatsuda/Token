//
//  token/afdko/opentype.cc
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

#include "token/afdko/opentype.h"

#include <cassert>
#include <fstream>
#include <ostream>
#include <string>

#include <boost/algorithm/string/join.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/range/adaptor/transformed.hpp>

#include "token/afdko/task.h"
#include "token/ufo.h"

namespace token {
namespace afdko {

namespace {

inline void write_name(std::ostream *stream,
                       unsigned int id,
                       const std::string& value) {
  if (!value.empty()) {
    assert(stream);
    *stream << "nameid " << id << " \"" << value << "\";\n";
    *stream << "nameid " << id << " 1 \"" << value << "\";\n";
  }
}

}  // namespace

bool createOpenTypeFont(const std::string& directory,
                        const std::string& input,
                        const std::string& output) {
  Task task;
  task.set_directory(directory);
  task.set_name("makeotf");
  task.set_arguments({"-r", "-f", input, "-o", output});
  return task.execute();
}

void createFeatures(const ufo::FontInfo& font_info,
                    const std::string& output) {
  const auto path = boost::filesystem::path(output) / "features";
  std::ofstream stream(path.string());
  assert(stream.good());
  const std::string endl = ";\n";
  stream << "languagesystem latn dflt" << endl;

  // kern
  stream << "feature kern {" << std::endl;
  stream << "include (kern.fea)" << std::endl;
  stream << "} kern;" << std::endl;

  // head
  stream << "table head {" << std::endl;
  stream << "FontRevision " << font_info.open_type_name_version << endl;
  stream << "} head;" << std::endl;

  // hhea
  stream << "table hhea {" << std::endl;
  stream << "Ascender " << font_info.open_type_hhea_ascender << endl;
  stream << "Descender " << font_info.open_type_hhea_descender << endl;
  stream << "LineGap " << font_info.open_type_hhea_line_gap << endl;
  stream << "} hhea;" << std::endl;

  // OS/2
  std::string (* const transformer)(unsigned int) = &std::to_string;
  stream << "table OS/2 {" << std::endl;
  stream << "FSType 0" << endl;
  stream << "Panose " << boost::algorithm::join(
      font_info.open_type_os2_panose |
      boost::adaptors::transformed(transformer), " ") << endl;
  stream << "TypoAscender " << font_info.open_type_os2_typo_ascender << endl;
  stream << "TypoDescender " << font_info.open_type_os2_typo_descender << endl;
  stream << "TypoLineGap " << font_info.open_type_os2_typo_line_gap << endl;
  stream << "winAscent " << font_info.open_type_os2_win_ascent << endl;
  stream << "winDescent " << font_info.open_type_os2_win_descent << endl;
  stream << "XHeight " << font_info.x_height << endl;
  stream << "CapHeight " << font_info.cap_height << endl;
  stream << "WeightClass " << font_info.open_type_os2_weight_class << endl;
  stream << "WidthClass " << font_info.open_type_os2_width_class << endl;
  stream << "Vendor \"" << font_info.open_type_os2_vendor_id << "\"" << endl;
  stream << "} OS/2;" << std::endl;

  // name
  stream << "table name {" << std::endl;
  write_name(&stream, 0, font_info.copyright);
  write_name(&stream, 7, font_info.trademark);
  write_name(&stream, 8, font_info.open_type_name_manufacturer);
  write_name(&stream, 9, font_info.open_type_name_designer);
  write_name(&stream, 10, font_info.open_type_name_description);
  write_name(&stream, 11, font_info.open_type_name_manufacturer_url);
  write_name(&stream, 12, font_info.open_type_name_designer_url);
  write_name(&stream, 13, font_info.open_type_name_license);
  write_name(&stream, 14, font_info.open_type_name_license_url);
  write_name(&stream, 16, font_info.open_type_name_preferred_family_name);
  write_name(&stream, 17, font_info.open_type_name_preferred_subfamily_name);
  write_name(&stream, 18, font_info.open_type_name_compatible_full_name);
  write_name(&stream, 19, font_info.open_type_name_sample_text);
  write_name(&stream, 21, font_info.open_type_name_wws_family_name);
  write_name(&stream, 22, font_info.open_type_name_wws_subfamily_name);
  stream << "} name;" << std::endl;
  stream.close();
}

void createFontMenuName(const ufo::FontInfo& font_info,
                        const std::string& output) {
  const auto path = boost::filesystem::path(output) / "FontMenuNameDB";
  std::ofstream stream(path.string());
  assert(stream.good());
  stream << "[" << font_info.postscript_font_name << "]" << std::endl;
  stream << "f=" << font_info.family_name << std::endl;
  stream << "s=" << font_info.style_name << std::endl;
  stream << "l=" << font_info.postscript_full_name << std::endl;
  stream << "m=1," << font_info.postscript_full_name << std::endl;
  stream.close();
}

void createGlyphOrderAndAlias(const ufo::Glyphs& glyphs,
                              const std::string& output) {
  const auto path = boost::filesystem::path(output) / "GlyphOrderAndAliasDB";
  std::ofstream stream(path.string());
  assert(stream.good());
  for (const auto& glyph : glyphs) {
    stream << glyph.name << " " << glyph.name << std::endl;
  }
  stream.close();
}

}  // namespace afdko
}  // namespace token
