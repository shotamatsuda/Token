//
//  token/ufo/fontinfo.cc
//
//  MIT License
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

#include "token/ufo/fontinfo.h"

#include <utility>

extern "C" {

#include <plist/plist.h>

}  // extern "C"

namespace token {
namespace ufo {

#pragma mark Move semantics

Fontinfo::Fontinfo(Fontinfo&& other)
    : path_(std::move(other.path_)),
      fontinfo_(other.fontinfo_),
      family_name(std::move(other.family_name)),
      style_name(std::move(other.style_name)),
      style_map_family_name(std::move(other.style_map_family_name)),
      style_map_style_name(std::move(other.style_map_style_name)),
      version_major(std::move(other.version_major)),
      version_minor(std::move(other.version_minor)),
      year(std::move(other.year)),
      copyright(std::move(other.copyright)),
      trademark(std::move(other.trademark)),
      units_per_em(std::move(other.units_per_em)),
      descender(std::move(other.descender)),
      x_height(std::move(other.x_height)),
      cap_height(std::move(other.cap_height)),
      ascender(std::move(other.ascender)),
      italic_angle(std::move(other.italic_angle)),
      note(std::move(other.note)),
      open_type_gasp_range_records(
          std::move(other.open_type_gasp_range_records)),
      open_type_head_created(std::move(other.open_type_head_created)),
      open_type_head_lowest_rec_ppem(
          std::move(other.open_type_head_lowest_rec_ppem)),
      open_type_head_flags(std::move(other.open_type_head_flags)),
      open_type_hhea_ascender(std::move(other.open_type_hhea_ascender)),
      open_type_hhea_descender(std::move(other.open_type_hhea_descender)),
      open_type_hhea_line_gap(std::move(other.open_type_hhea_line_gap)),
      open_type_hhea_caret_slope_rise(
          std::move(other.open_type_hhea_caret_slope_rise)),
      open_type_hhea_caret_slope_run(
          std::move(other.open_type_hhea_caret_slope_run)),
      open_type_hhea_caret_offset(std::move(other.open_type_hhea_caret_offset)),
      open_type_name_designer(std::move(other.open_type_name_designer)),
      open_type_name_designer_url(std::move(other.open_type_name_designer_url)),
      open_type_name_manufacturer(std::move(other.open_type_name_manufacturer)),
      open_type_name_manufacturer_url(
          std::move(other.open_type_name_manufacturer_url)),
      open_type_name_license(std::move(other.open_type_name_license)),
      open_type_name_license_url(std::move(other.open_type_name_license_url)),
      open_type_name_version(std::move(other.open_type_name_version)),
      open_type_name_unique_id(std::move(other.open_type_name_unique_id)),
      open_type_name_description(std::move(other.open_type_name_description)),
      open_type_name_preferred_family_name(
          std::move(other.open_type_name_preferred_family_name)),
      open_type_name_preferred_subfamily_name(
          std::move(other.open_type_name_preferred_subfamily_name)),
      open_type_name_compatible_full_name(
          std::move(other.open_type_name_compatible_full_name)),
      open_type_name_sample_text(std::move(other.open_type_name_sample_text)),
      open_type_name_wws_family_name(
          std::move(other.open_type_name_wws_family_name)),
      open_type_name_wws_subfamily_name(
          std::move(other.open_type_name_wws_subfamily_name)),
      open_type_name_records(std::move(other.open_type_name_records)),
      open_type_os2_width_class(std::move(other.open_type_os2_width_class)),
      open_type_os2_weight_class(std::move(other.open_type_os2_weight_class)),
      open_type_os2_selection(std::move(other.open_type_os2_selection)),
      open_type_os2_vendor_id(std::move(other.open_type_os2_vendor_id)),
      open_type_os2_panose(std::move(other.open_type_os2_panose)),
      open_type_os2_family_class(std::move(other.open_type_os2_family_class)),
      open_type_os2_unicode_ranges(
          std::move(other.open_type_os2_unicode_ranges)),
      open_type_os2_code_page_ranges(
          std::move(other.open_type_os2_code_page_ranges)),
      open_type_os2_typo_ascender(std::move(other.open_type_os2_typo_ascender)),
      open_type_os2_typo_descender(
          std::move(other.open_type_os2_typo_descender)),
      open_type_os2_typo_line_gap(std::move(other.open_type_os2_typo_line_gap)),
      open_type_os2_win_ascent(std::move(other.open_type_os2_win_ascent)),
      open_type_os2_win_descent(std::move(other.open_type_os2_win_descent)),
      open_type_os2_type(std::move(other.open_type_os2_type)),
      open_type_os2_subscript_x_size(
          std::move(other.open_type_os2_subscript_x_size)),
      open_type_os2_subscript_y_size(
          std::move(other.open_type_os2_subscript_y_size)),
      open_type_os2_subscript_x_offset(
          std::move(other.open_type_os2_subscript_x_offset)),
      open_type_os2_subscript_y_offset(
          std::move(other.open_type_os2_subscript_y_offset)),
      open_type_os2_superscript_x_size(
          std::move(other.open_type_os2_superscript_x_size)),
      open_type_os2_superscript_y_size(
          std::move(other.open_type_os2_superscript_y_size)),
      open_type_os2_superscript_x_offset(
          std::move(other.open_type_os2_superscript_x_offset)),
      open_type_os2_superscript_y_offset(
          std::move(other.open_type_os2_superscript_y_offset)),
      open_type_os2_strikeout_size(
          std::move(other.open_type_os2_strikeout_size)),
      open_type_os2_strikeout_position(
          std::move(other.open_type_os2_strikeout_position)),
      open_type_vhea_vert_typo_ascender(
          std::move(other.open_type_vhea_vert_typo_ascender)),
      open_type_vhea_vert_typo_descender(
          std::move(other.open_type_vhea_vert_typo_descender)),
      open_type_vhea_vert_typo_line_gap(
          std::move(other.open_type_vhea_vert_typo_line_gap)),
      open_type_vhea_caret_slope_rise(
          std::move(other.open_type_vhea_caret_slope_rise)),
      open_type_vhea_caret_slope_run(
          std::move(other.open_type_vhea_caret_slope_run)),
      open_type_vhea_caret_offset(std::move(other.open_type_vhea_caret_offset)),
      postscript_font_name(std::move(other.postscript_font_name)),
      postscript_full_name(std::move(other.postscript_full_name)),
      postscript_slant_angle(std::move(other.postscript_slant_angle)),
      postscript_unique_id(std::move(other.postscript_unique_id)),
      postscript_underline_thickness(
          std::move(other.postscript_underline_thickness)),
      postscript_underline_position(
          std::move(other.postscript_underline_position)),
      postscript_is_fixed_pitch(std::move(other.postscript_is_fixed_pitch)),
      postscript_blue_values(std::move(other.postscript_blue_values)),
      postscript_other_blues(std::move(other.postscript_other_blues)),
      postscript_family_blues(std::move(other.postscript_family_blues)),
      postscript_family_other_blues(
          std::move(other.postscript_family_other_blues)),
      postscript_stem_snap_h(std::move(other.postscript_stem_snap_h)),
      postscript_stem_snap_v(std::move(other.postscript_stem_snap_v)),
      postscript_blue_fuzz(std::move(other.postscript_blue_fuzz)),
      postscript_blue_shift(std::move(other.postscript_blue_shift)),
      postscript_blue_scale(std::move(other.postscript_blue_scale)),
      postscript_force_bold(std::move(other.postscript_force_bold)),
      postscript_default_width_x(std::move(other.postscript_default_width_x)),
      postscript_nominal_width_x(std::move(other.postscript_nominal_width_x)),
      postscript_weight_name(std::move(other.postscript_weight_name)),
      postscript_default_character(
          std::move(other.postscript_default_character)),
      postscript_windows_character_set(
          std::move(other.postscript_windows_character_set)),
      macintosh_fond_family_id(std::move(other.macintosh_fond_family_id)),
      macintosh_fond_name(std::move(other.macintosh_fond_name)),
      woff_major_version(std::move(other.woff_major_version)),
      woff_minor_version(std::move(other.woff_minor_version)),
      woff_metadata_unique_id(std::move(other.woff_metadata_unique_id)),
      woff_metadata_vendor(std::move(other.woff_metadata_vendor)),
      woff_metadata_credits(std::move(other.woff_metadata_credits)),
      woff_metadata_description(std::move(other.woff_metadata_description)),
      woff_metadata_license(std::move(other.woff_metadata_license)),
      woff_metadata_copyright(std::move(other.woff_metadata_copyright)),
      woff_metadata_trademark(std::move(other.woff_metadata_trademark)),
      woff_metadata_licensee(std::move(other.woff_metadata_licensee)),
      woff_metadata_extensions(std::move(other.woff_metadata_extensions)),
      guidelines(std::move(other.guidelines)) {
  other.fontinfo_ = nullptr;
}

Fontinfo& Fontinfo::operator=(Fontinfo&& other) {
  if (&other != this) {
    std::swap(path_, other.path_);
    std::swap(fontinfo_, other.fontinfo_);
    std::swap(family_name, other.family_name);
    std::swap(style_name, other.style_name);
    std::swap(style_map_family_name, other.style_map_family_name);
    std::swap(style_map_style_name, other.style_map_style_name);
    std::swap(version_major, other.version_major);
    std::swap(version_minor, other.version_minor);
    std::swap(year, other.year);
    std::swap(copyright, other.copyright);
    std::swap(trademark, other.trademark);
    std::swap(units_per_em, other.units_per_em);
    std::swap(descender, other.descender);
    std::swap(x_height, other.x_height);
    std::swap(cap_height, other.cap_height);
    std::swap(ascender, other.ascender);
    std::swap(italic_angle, other.italic_angle);
    std::swap(note, other.note);
    std::swap(open_type_gasp_range_records, other.open_type_gasp_range_records);
    std::swap(open_type_head_created, other.open_type_head_created);
    std::swap(open_type_head_lowest_rec_ppem,
              other.open_type_head_lowest_rec_ppem);
    std::swap(open_type_head_flags, other.open_type_head_flags);
    std::swap(open_type_hhea_ascender, other.open_type_hhea_ascender);
    std::swap(open_type_hhea_descender, other.open_type_hhea_descender);
    std::swap(open_type_hhea_line_gap, other.open_type_hhea_line_gap);
    std::swap(open_type_hhea_caret_slope_rise,
              other.open_type_hhea_caret_slope_rise);
    std::swap(open_type_hhea_caret_slope_run,
              other.open_type_hhea_caret_slope_run);
    std::swap(open_type_hhea_caret_offset, other.open_type_hhea_caret_offset);
    std::swap(open_type_name_designer, other.open_type_name_designer);
    std::swap(open_type_name_designer_url, other.open_type_name_designer_url);
    std::swap(open_type_name_manufacturer, other.open_type_name_manufacturer);
    std::swap(open_type_name_manufacturer_url,
              other.open_type_name_manufacturer_url);
    std::swap(open_type_name_license, other.open_type_name_license);
    std::swap(open_type_name_license_url, other.open_type_name_license_url);
    std::swap(open_type_name_version, other.open_type_name_version);
    std::swap(open_type_name_unique_id, other.open_type_name_unique_id);
    std::swap(open_type_name_description, other.open_type_name_description);
    std::swap(open_type_name_preferred_family_name,
              other.open_type_name_preferred_family_name);
    std::swap(open_type_name_preferred_subfamily_name,
              other.open_type_name_preferred_subfamily_name);
    std::swap(open_type_name_compatible_full_name,
              other.open_type_name_compatible_full_name);
    std::swap(open_type_name_sample_text, other.open_type_name_sample_text);
    std::swap(open_type_name_wws_family_name,
              other.open_type_name_wws_family_name);
    std::swap(open_type_name_wws_subfamily_name,
              other.open_type_name_wws_subfamily_name);
    std::swap(open_type_name_records, other.open_type_name_records);
    std::swap(open_type_os2_width_class, other.open_type_os2_width_class);
    std::swap(open_type_os2_weight_class, other.open_type_os2_weight_class);
    std::swap(open_type_os2_selection, other.open_type_os2_selection);
    std::swap(open_type_os2_vendor_id, other.open_type_os2_vendor_id);
    std::swap(open_type_os2_panose, other.open_type_os2_panose);
    std::swap(open_type_os2_family_class, other.open_type_os2_family_class);
    std::swap(open_type_os2_unicode_ranges, other.open_type_os2_unicode_ranges);
    std::swap(open_type_os2_code_page_ranges,
              other.open_type_os2_code_page_ranges);
    std::swap(open_type_os2_typo_ascender, other.open_type_os2_typo_ascender);
    std::swap(open_type_os2_typo_descender, other.open_type_os2_typo_descender);
    std::swap(open_type_os2_typo_line_gap, other.open_type_os2_typo_line_gap);
    std::swap(open_type_os2_win_ascent, other.open_type_os2_win_ascent);
    std::swap(open_type_os2_win_descent, other.open_type_os2_win_descent);
    std::swap(open_type_os2_type, other.open_type_os2_type);
    std::swap(open_type_os2_subscript_x_size,
              other.open_type_os2_subscript_x_size);
    std::swap(open_type_os2_subscript_y_size,
              other.open_type_os2_subscript_y_size);
    std::swap(open_type_os2_subscript_x_offset,
              other.open_type_os2_subscript_x_offset);
    std::swap(open_type_os2_subscript_y_offset,
              other.open_type_os2_subscript_y_offset);
    std::swap(open_type_os2_superscript_x_size,
              other.open_type_os2_superscript_x_size);
    std::swap(open_type_os2_superscript_y_size,
              other.open_type_os2_superscript_y_size);
    std::swap(open_type_os2_superscript_x_offset,
              other.open_type_os2_superscript_x_offset);
    std::swap(open_type_os2_superscript_y_offset,
              other.open_type_os2_superscript_y_offset);
    std::swap(open_type_os2_strikeout_size, other.open_type_os2_strikeout_size);
    std::swap(open_type_os2_strikeout_position,
              other.open_type_os2_strikeout_position);
    std::swap(open_type_vhea_vert_typo_ascender,
              other.open_type_vhea_vert_typo_ascender);
    std::swap(open_type_vhea_vert_typo_descender,
              other.open_type_vhea_vert_typo_descender);
    std::swap(open_type_vhea_vert_typo_line_gap,
              other.open_type_vhea_vert_typo_line_gap);
    std::swap(open_type_vhea_caret_slope_rise,
              other.open_type_vhea_caret_slope_rise);
    std::swap(open_type_vhea_caret_slope_run,
              other.open_type_vhea_caret_slope_run);
    std::swap(open_type_vhea_caret_offset, other.open_type_vhea_caret_offset);
    std::swap(postscript_font_name, other.postscript_font_name);
    std::swap(postscript_full_name, other.postscript_full_name);
    std::swap(postscript_slant_angle, other.postscript_slant_angle);
    std::swap(postscript_unique_id, other.postscript_unique_id);
    std::swap(postscript_underline_thickness,
              other.postscript_underline_thickness);
    std::swap(postscript_underline_position,
              other.postscript_underline_position);
    std::swap(postscript_is_fixed_pitch, other.postscript_is_fixed_pitch);
    std::swap(postscript_blue_values, other.postscript_blue_values);
    std::swap(postscript_other_blues, other.postscript_other_blues);
    std::swap(postscript_family_blues, other.postscript_family_blues);
    std::swap(postscript_family_other_blues,
              other.postscript_family_other_blues);
    std::swap(postscript_stem_snap_h, other.postscript_stem_snap_h);
    std::swap(postscript_stem_snap_v, other.postscript_stem_snap_v);
    std::swap(postscript_blue_fuzz, other.postscript_blue_fuzz);
    std::swap(postscript_blue_shift, other.postscript_blue_shift);
    std::swap(postscript_blue_scale, other.postscript_blue_scale);
    std::swap(postscript_force_bold, other.postscript_force_bold);
    std::swap(postscript_default_width_x, other.postscript_default_width_x);
    std::swap(postscript_nominal_width_x, other.postscript_nominal_width_x);
    std::swap(postscript_weight_name, other.postscript_weight_name);
    std::swap(postscript_default_character, other.postscript_default_character);
    std::swap(postscript_windows_character_set,
              other.postscript_windows_character_set);
    std::swap(macintosh_fond_family_id, other.macintosh_fond_family_id);
    std::swap(macintosh_fond_name, other.macintosh_fond_name);
    std::swap(woff_major_version, other.woff_major_version);
    std::swap(woff_minor_version, other.woff_minor_version);
    std::swap(woff_metadata_unique_id, other.woff_metadata_unique_id);
    std::swap(woff_metadata_vendor, other.woff_metadata_vendor);
    std::swap(woff_metadata_credits, other.woff_metadata_credits);
    std::swap(woff_metadata_description, other.woff_metadata_description);
    std::swap(woff_metadata_license, other.woff_metadata_license);
    std::swap(woff_metadata_copyright, other.woff_metadata_copyright);
    std::swap(woff_metadata_trademark, other.woff_metadata_trademark);
    std::swap(woff_metadata_licensee, other.woff_metadata_licensee);
    std::swap(woff_metadata_extensions, other.woff_metadata_extensions);
    std::swap(guidelines, other.guidelines);
  }
  return *this;
}

Fontinfo::~Fontinfo() {
  if (fontinfo_) {
    plist_free(fontinfo_);
  }
}

}  // namespace ufo
}  // namespace token
