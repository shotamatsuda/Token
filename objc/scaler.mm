//
//  scaler.mm
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

#import <Foundation/Foundation.h>

#include <cmath>
#include <cstdlib>
#include <string>

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

static void scaleCapHeightToUnitsPerEM(const std::string& path) {
  token::ufo::FontInfo info(path);
  token::ufo::Glyphs glyphs(path);

  // We are going to scale everything by this value
  const auto scale = info.units_per_em / info.cap_height;

  // Key metrices. Although the distance between ascender and descender is
  // recommended to be units per em, the goal here is to make font size to
  // match cap height. Both ascender and descender will exceed em box.
  const auto ascender = std::ceil(info.ascender * scale);
  const auto cap_height = std::round(info.cap_height * scale);
  const auto x_height = std::round(info.x_height * scale);
  const auto descender = std::floor(info.descender * scale);
  const auto line_gap = std::ceil(info.open_type_hhea_line_gap * scale);

  // Dimension Information
  info.ascender = ascender;
  info.cap_height = cap_height;
  info.x_height = x_height;
  info.descender = descender;

  // OpenType hhea Table Fields
  info.open_type_hhea_ascender = ascender;
  info.open_type_hhea_descender = descender;
  info.open_type_hhea_line_gap = line_gap;

  // OpenType OS/2 Table Fields
  info.open_type_os2_typo_ascender = ascender;
  info.open_type_os2_typo_descender = descender;
  info.open_type_os2_typo_line_gap = line_gap;
  info.open_type_os2_win_ascent = ascender;
  info.open_type_os2_win_descent = -descender;
  scale_if_exists(&info.open_type_os2_subscript_x_size, scale);
  scale_if_exists(&info.open_type_os2_subscript_y_size, scale);
  scale_if_exists(&info.open_type_os2_subscript_x_offset, scale);
  scale_if_exists(&info.open_type_os2_subscript_y_offset, scale);
  scale_if_exists(&info.open_type_os2_superscript_x_size, scale);
  scale_if_exists(&info.open_type_os2_superscript_y_size, scale);
  scale_if_exists(&info.open_type_os2_superscript_x_offset, scale);
  scale_if_exists(&info.open_type_os2_superscript_y_offset, scale);
  scale_if_exists(&info.open_type_os2_strikeout_size, scale);
  scale_if_exists(&info.open_type_os2_strikeout_position, scale);

  // OpenType vhea Table Fields
  scale_if_exists(&info.open_type_vhea_vert_typo_ascender, scale);
  scale_if_exists(&info.open_type_vhea_vert_typo_descender, scale);
  scale_if_exists(&info.open_type_vhea_vert_typo_line_gap, scale);

  // PostScript Specific Data
  scale_if_exists(&info.postscript_underline_thickness, scale);
  scale_if_exists(&info.postscript_underline_position, scale);
  scale_if_exists(&info.postscript_blue_values, scale);
  scale_if_exists(&info.postscript_other_blues, scale);
  scale_if_exists(&info.postscript_family_blues, scale);
  scale_if_exists(&info.postscript_family_other_blues, scale);
  scale_if_exists(&info.postscript_stem_snap_h, scale);
  scale_if_exists(&info.postscript_stem_snap_v, scale);
  scale_if_exists(&info.postscript_default_width_x, scale);
  scale_if_exists(&info.postscript_nominal_width_x, scale);
  info.save(path);

  // Glyphs
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

int main(int argc, const char *argv[]) {
  if (argc < 3) {
    return EXIT_FAILURE;
  }
  NSString *source = [NSString stringWithUTF8String:argv[1]];
  NSString *destination = [NSString stringWithUTF8String:argv[2]];
  NSFileManager *manager = [NSFileManager defaultManager];
  NSError *error = nil;
  if ([manager fileExistsAtPath:destination]) {
    if (![manager removeItemAtPath:destination error:&error]) {
      return EXIT_FAILURE;
    }
  }
  if (![manager copyItemAtPath:source toPath:destination error:&error]) {
    return EXIT_FAILURE;
  }
  scaleCapHeightToUnitsPerEM(destination.UTF8String);
  return EXIT_SUCCESS;
}
