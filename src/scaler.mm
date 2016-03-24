//
//  scaler.mm
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

#import <Foundation/Foundation.h>

extern "C" {

#include <plist/plist.h>

}  // extern "C"

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include <boost/filesystem/path.hpp>

#include "token/ufo.h"

namespace {

template <class T, class U>
inline void scale_if_exists(T *value, U scale) {
  *value = std::round(*value * scale);
}

template <class T, class U>
inline void scale_if_exists(token::ufo::Optional<T> *value, U scale) {
  if (value->exists()) {
    *value = std::round(**value * scale);
  }
}

template <class T, class U>
inline void scale_if_exists(std::vector<T> *values, U scale) {
  for (auto& value : *values) {
    scale_if_exists(&value, scale);
  }
}

}  // namespace

static void scaleFontInfo(const std::string& path, double scale) {
  token::ufo::FontInfo font_info(path);

  // Key metrices. Although the distance between ascender and descender is
  // recommended to be units per em, the goal here is to make font size to
  // match cap height. Both ascender and descender will exceed em box.
  const auto ascender = std::ceil(font_info.ascender * scale);
  const auto cap_height = std::round(font_info.cap_height * scale);
  const auto x_height = std::round(font_info.x_height * scale);
  const auto descender = std::floor(font_info.descender * scale);
  const auto line_gap = std::ceil(font_info.open_type_hhea_line_gap * scale);

  // Dimension Information
  font_info.ascender = ascender;
  font_info.cap_height = cap_height;
  font_info.x_height = x_height;
  font_info.descender = descender;

  // OpenType hhea Table Fields
  font_info.open_type_hhea_ascender = ascender;
  font_info.open_type_hhea_descender = descender;
  font_info.open_type_hhea_line_gap = line_gap;

  // OpenType OS/2 Table Fields
  font_info.open_type_os2_typo_ascender = ascender;
  font_info.open_type_os2_typo_descender = descender;
  font_info.open_type_os2_typo_line_gap = line_gap;
  font_info.open_type_os2_win_ascent = ascender;
  font_info.open_type_os2_win_descent = -descender;
  scale_if_exists(&font_info.open_type_os2_subscript_x_size, scale);
  scale_if_exists(&font_info.open_type_os2_subscript_y_size, scale);
  scale_if_exists(&font_info.open_type_os2_subscript_x_offset, scale);
  scale_if_exists(&font_info.open_type_os2_subscript_y_offset, scale);
  scale_if_exists(&font_info.open_type_os2_superscript_x_size, scale);
  scale_if_exists(&font_info.open_type_os2_superscript_y_size, scale);
  scale_if_exists(&font_info.open_type_os2_superscript_x_offset, scale);
  scale_if_exists(&font_info.open_type_os2_superscript_y_offset, scale);
  scale_if_exists(&font_info.open_type_os2_strikeout_size, scale);
  scale_if_exists(&font_info.open_type_os2_strikeout_position, scale);

  // OpenType vhea Table Fields
  scale_if_exists(&font_info.open_type_vhea_vert_typo_ascender, scale);
  scale_if_exists(&font_info.open_type_vhea_vert_typo_descender, scale);
  scale_if_exists(&font_info.open_type_vhea_vert_typo_line_gap, scale);

  // PostScript Specific Data
  scale_if_exists(&font_info.postscript_underline_thickness, scale);
  scale_if_exists(&font_info.postscript_underline_position, scale);
  scale_if_exists(&font_info.postscript_blue_values, scale);
  scale_if_exists(&font_info.postscript_other_blues, scale);
  scale_if_exists(&font_info.postscript_family_blues, scale);
  scale_if_exists(&font_info.postscript_family_other_blues, scale);
  scale_if_exists(&font_info.postscript_stem_snap_h, scale);
  scale_if_exists(&font_info.postscript_stem_snap_v, scale);
  scale_if_exists(&font_info.postscript_default_width_x, scale);
  scale_if_exists(&font_info.postscript_nominal_width_x, scale);
  font_info.save(path);
}

static void scaleGlyphs(const std::string& path, double scale) {
  token::ufo::Glyphs glyphs(path);
  const auto glyphs_path = boost::filesystem::path(path) / "glyphs";
  for (auto& glyph : glyphs) {
    double x_offset{};  // Compensate for rounding error
    if (glyph.advance.exists()) {
      const auto scaled_width = glyph.advance->width * scale;
      const auto rounded_width = std::round(scaled_width);
      x_offset = (rounded_width - scaled_width) / 2.0;
      glyph.advance->width = rounded_width;
      glyph.advance->height = std::round(glyph.advance->height * scale);
    }
    if (glyph.image.exists()) {
      glyph.image->x_scale *= scale;
      glyph.image->xy_scale *= scale;
      glyph.image->yx_scale *= scale;
      glyph.image->y_scale *= scale;
      glyph.image->x_offset *= scale;
      glyph.image->y_offset *= scale;
      glyph.image->x_offset += x_offset;
    }
    for (auto& guideline : glyph.guidelines) {
      guideline.x *= scale;
      guideline.y *= scale;
      guideline.x += x_offset;
    }
    for (auto& anchor : glyph.anchors) {
      anchor.x *= scale;
      anchor.y *= scale;
      anchor.x += x_offset;
    }
    if (glyph.outline.exists()) {
      for (auto& component : glyph.outline->components) {
        component.x_scale *= scale;
        component.xy_scale *= scale;
        component.yx_scale *= scale;
        component.y_scale *= scale;
        component.x_offset *= scale;
        component.y_offset *= scale;
        component.x_offset += x_offset;
      }
      for (auto& contour : glyph.outline->contours) {
        for (auto& point : contour.points) {
          point.x *= scale;
          point.y *= scale;
          point.x += x_offset;
        }
      }
    }
    glyph.save((glyphs_path / glyphs.filename(glyph.name)).string());
  }
}

static void scaleKerning(const std::string& path, double scale) {
  const auto kerning_path = boost::filesystem::path(path) / "kerning.plist";
  std::ifstream stream(kerning_path.string());
  token::ufo::PropertyList plist(&stream);
  plist_dict_iter first_itr{};
  plist_dict_new_iter(plist, &first_itr);
  const auto first_size = plist_dict_get_size(plist);
  for (std::uint32_t i{}; i < first_size; ++i) {
    char *first_key{};
    plist_t first_item{};
    plist_dict_next_item(plist, first_itr, &first_key, &first_item);
    assert(first_item);
    assert(plist_get_node_type(first_item) == PLIST_DICT);
    plist_dict_iter second_itr{};
    plist_dict_new_iter(first_item, &second_itr);
    const auto second_size = plist_dict_get_size(first_item);
    for (std::uint32_t i{}; i < second_size; ++i) {
      char *second_key{};
      plist_t second_item{};
      plist_dict_next_item(first_item, second_itr, &second_key, &second_item);
      assert(second_item);
      assert(plist_get_node_type(second_item) == PLIST_UINT);
      std::uint64_t raw_value{};
      plist_get_uint_val(second_item, &raw_value);
      const auto value = *reinterpret_cast<std::int64_t *>(&raw_value);
      const auto scaled_item = plist_new_uint(std::round(value * scale));
      plist_dict_set_item(first_item, second_key, scaled_item);
      std::free(second_key);
    }
    std::free(second_itr);
    std::free(first_key);
  }
  std::free(first_itr);
  std::ofstream ostream(kerning_path.string());
  plist.save(&ostream);
}

int main(int argc, const char *argv[]) {
  if (argc < 3) {
    return EXIT_FAILURE;
  }
  NSURL *source = [NSURL fileURLWithPath:
      [NSString stringWithUTF8String:argv[1]]];
  NSURL *destination = [NSURL fileURLWithPath:
      [NSString stringWithUTF8String:argv[2]]];
  NSFileManager *manager = [NSFileManager defaultManager];
  NSError *error = nil;
  if ([destination checkResourceIsReachableAndReturnError:nil]) {
    if (![manager removeItemAtURL:destination error:&error]) {
      return EXIT_FAILURE;
    }
  }
  if (![manager copyItemAtURL:source toURL:destination error:&error]) {
    return EXIT_FAILURE;
  }
  const std::string source_font_path(source.path.UTF8String);
  const std::string destination_font_path(destination.path.UTF8String);

  // We are going to scale everything by this value
  token::ufo::FontInfo font_info(source_font_path);
  const auto scale = font_info.units_per_em / font_info.cap_height;
  scaleFontInfo(destination_font_path, scale);
  scaleGlyphs(destination_font_path, scale);
  scaleKerning(destination_font_path, scale);
  return EXIT_SUCCESS;
}
