//
//  token/ufo/fontinfo.h
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

#pragma once
#ifndef TOKEN_UFO_FONTINFO_H_
#define TOKEN_UFO_FONTINFO_H_

#include <map>
#include <string>
#include <utility>
#include <vector>

#include "token/ufo/gasp_range_record.h"
#include "token/ufo/guideline.h"
#include "token/ufo/name_record.h"
#include "token/ufo/woff.h"

namespace token {
namespace ufo {

class Fontinfo final {
 public:
  Fontinfo() = default;

  // Copy semantics
  Fontinfo(const Fontinfo& other) = default;
  Fontinfo& operator=(const Fontinfo& other) = default;

 public:
  // Generic Identification Information
  std::string family_name;
  std::string style_name;
  std::string style_map_family_name;
  std::string style_map_style_name;
  std::pair<bool, int> version_major;
  std::pair<bool, unsigned int> version_minor;
  std::pair<bool, int> year;

  // Generic Legal Information
  std::string copyright;
  std::string trademark;

  // Generic Dimension Information
  std::pair<bool, double> units_per_em;
  std::pair<bool, double> descender;
  std::pair<bool, double> x_height;
  std::pair<bool, double> cap_height;
  std::pair<bool, double> ascender;
  std::pair<bool, double> italic_angle;

  // Generic Miscellaneous Information
  std::string note;

  // OpenType gasp Table Fields
  std::vector<GASPRangeRecord> open_type_gasp_range_records;

  // OpenType head Table Fields
  std::string open_type_head_created;
  std::pair<bool, unsigned int> open_type_head_lowest_rec_ppem;
  std::vector<unsigned int> open_type_head_flags;

  // OpenType hhea Table Fields
  std::pair<bool, int> open_type_hhea_ascender;
  std::pair<bool, int> open_type_hhea_descender;
  std::pair<bool, int> open_type_hhea_line_gap;
  std::pair<bool, int> open_type_hhea_caret_slope_rise;
  std::pair<bool, int> open_type_hhea_caret_slope_run;
  std::pair<bool, int> open_type_hhea_caret_offset;

  // OpenType Name Table Fields
  std::string open_type_name_designer;
  std::string open_type_name_designer_url;
  std::string open_type_name_manufacturer;
  std::string open_type_name_manufacturer_url;
  std::string open_type_name_license;
  std::string open_type_name_license_url;
  std::string open_type_name_version;
  std::string open_type_name_unique_id;
  std::string open_type_name_description;
  std::string open_type_name_preferred_family_name;
  std::string open_type_name_preferred_subfamily_name;
  std::string open_type_name_compatible_full_name;
  std::string open_type_name_sample_text;
  std::string open_type_name_wws_family_name;
  std::string open_type_name_wws_subfamily_name;
  std::pair<bool, std::vector<NameRecord>> open_type_name_records;

  // OpenType OS/2 Table Fields
  std::pair<bool, int> open_type_os2_width_class;
  std::pair<bool, int> open_type_os2_weight_class;
  std::vector<unsigned int> open_type_os2_selection;
  std::string open_type_os2_vendor_id;
  std::vector<unsigned int> open_type_os2_panose;
  std::vector<unsigned int> open_type_os2_family_class;
  std::vector<unsigned int> open_type_os2_unicode_ranges;
  std::vector<unsigned int> open_type_os2_code_page_ranges;
  std::pair<bool, int> open_type_os2_typo_ascender;
  std::pair<bool, int> open_type_os2_typo_descender;
  std::pair<bool, int> open_type_os2_typo_line_gap;
  std::pair<bool, unsigned int> open_type_os2_win_ascent;
  std::pair<bool, unsigned int> open_type_os2_win_descent;
  std::vector<unsigned int> open_type_os2_type;
  std::pair<bool, int> open_type_os2_subscript_x_size;
  std::pair<bool, int> open_type_os2_subscript_y_size;
  std::pair<bool, int> open_type_os2_subscript_x_offset;
  std::pair<bool, int> open_type_os2_subscript_y_offset;
  std::pair<bool, int> open_type_os2_superscript_x_size;
  std::pair<bool, int> open_type_os2_superscript_y_size;
  std::pair<bool, int> open_type_os2_superscript_x_offset;
  std::pair<bool, int> open_type_os2_superscript_y_offset;
  std::pair<bool, int> open_type_os2_strikeout_size;
  std::pair<bool, int> open_type_os2_strikeout_position;

  // OpenType vhea Table Fields
  std::pair<bool, int> open_type_vhea_vert_typo_ascender;
  std::pair<bool, int> open_type_vhea_vert_typo_descender;
  std::pair<bool, int> open_type_vhea_vert_typo_line_gap;
  std::pair<bool, int> open_type_vhea_caret_slope_rise;
  std::pair<bool, int> open_type_vhea_caret_slope_run;
  std::pair<bool, int> open_type_vhea_caret_offset;

  // PostScript Specific Data
  std::string postscript_font_name;
  std::string postscript_full_name;
  std::pair<bool, double> postscript_slant_angle;
  std::pair<bool, int> postscript_unique_id;
  std::pair<bool, double> postscript_underline_thickness;
  std::pair<bool, double> postscript_underline_position;
  std::pair<bool, bool> postscript_is_fixed_pitch;
  std::vector<double> postscript_blue_values;
  std::vector<double> postscript_other_blues;
  std::vector<double> postscript_family_blues;
  std::vector<double> postscript_family_other_blues;
  std::vector<double> postscript_stem_snap_h;
  std::vector<double> postscript_stem_snap_v;
  std::pair<bool, double> postscript_blue_fuzz;
  std::pair<bool, double> postscript_blue_shift;
  std::pair<bool, float> postscript_blue_scale;
  std::pair<bool, bool> postscript_force_bold;
  std::pair<bool, double> postscript_default_width_x;
  std::pair<bool, double> postscript_nominal_width_x;
  std::string postscript_weight_name;
  std::string postscript_default_character;
  std::pair<bool, int> postscript_windows_character_set;

  // Macintosh FOND Resource Data
  std::pair<bool, int> macintosh_fond_family_id;
  std::string macintosh_fond_name;

  // WOFF Data
  std::pair<bool, unsigned int> woff_major_version;
  std::pair<bool, unsigned int> woff_minor_version;
  std::map<std::string, woff::metadata::UniqueID> woff_metadata_unique_id;
  std::map<std::string, woff::metadata::Vendor> woff_metadata_vendor;
  std::map<std::string, woff::metadata::Credits> woff_metadata_credits;
  std::map<std::string, woff::metadata::Description> woff_metadata_description;
  std::map<std::string, woff::metadata::License> woff_metadata_license;
  std::map<std::string, woff::metadata::Copyright> woff_metadata_copyright;
  std::map<std::string, woff::metadata::Trademark> woff_metadata_trademark;
  std::map<std::string, woff::metadata::Licensee> woff_metadata_licensee;
  std::vector<woff::metadata::Extension> woff_metadata_extensions;

  // Guidelines
  std::vector<Guideline> guidelines;
};

}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_FONTINFO_H_
