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

#pragma once
#ifndef TOKEN_UFO_FONT_INFO_H_
#define TOKEN_UFO_FONT_INFO_H_

#include <istream>
#include <map>
#include <ostream>
#include <string>
#include <vector>

#include "token/ufo/gasp_range_record.h"
#include "token/ufo/guideline.h"
#include "token/ufo/name_record.h"
#include "token/ufo/optional.h"
#include "token/ufo/property_list.h"
#include "token/ufo/woff.h"

namespace token {
namespace ufo {

class FontInfo final {
 public:
  FontInfo() = default;
  explicit FontInfo(const std::string& ufo_path);
  explicit FontInfo(std::istream& stream);

  // Copy semantics
  FontInfo(const FontInfo&) = default;
  FontInfo& operator=(const FontInfo&) = default;

  // Opening and saving
  bool open(const std::string& path);
  bool open(std::istream& stream);
  bool save(const std::string& path) const;
  bool save(std::ostream& stream) const;

 private:
  // Reading from property list
  void readIdentificationInformation(const PropertyList& plist);
  void readLegalInformation(const PropertyList& plist);
  void readDimensionInformation(const PropertyList& plist);
  void readMiscellaneousInformation(const PropertyList& plist);
  void readOpenTypeGASPTableFields(const PropertyList& plist);
  void readOpenTypeHEADTableFields(const PropertyList& plist);
  void readOpenTypeHHEATableFields(const PropertyList& plist);
  void readOpenTypeNameTableFields(const PropertyList& plist);
  void readOpenTypeOS2TableFields(const PropertyList& plist);
  void readOpenTypeVHEATableFields(const PropertyList& plist);
  void readPostScriptSpecificData(const PropertyList& plist);
  void readMacintoshFONDResourceData(const PropertyList& plist);
  void readWOFFData(const PropertyList& plist);
  void readGuidelines(const PropertyList& plist);

  // Writing to property list
  void writeIdentificationInformation(const PropertyList& plist) const;
  void writeLegalInformation(const PropertyList& plist) const;
  void writeDimensionInformation(const PropertyList& plist) const;
  void writeMiscellaneousInformation(const PropertyList& plist) const;
  void writeOpenTypeGASPTableFields(const PropertyList& plist) const;
  void writeOpenTypeHEADTableFields(const PropertyList& plist) const;
  void writeOpenTypeHHEATableFields(const PropertyList& plist) const;
  void writeOpenTypeNameTableFields(const PropertyList& plist) const;
  void writeOpenTypeOS2TableFields(const PropertyList& plist) const;
  void writeOpenTypeVHEATableFields(const PropertyList& plist) const;
  void writePostScriptSpecificData(const PropertyList& plist) const;
  void writeMacintoshFONDResourceData(const PropertyList& plist) const;
  void writeWOFFData(const PropertyList& plist) const;
  void writeGuidelines(const PropertyList& plist) const;

 public:
  // Identification Information
  std::string family_name;
  std::string style_name;
  std::string style_map_family_name;
  std::string style_map_style_name;
  Optional<int> version_major;
  Optional<unsigned int> version_minor;
  Optional<int> year;

  // Legal Information
  std::string copyright;
  std::string trademark;

  // Dimension Information
  Optional<double> units_per_em;
  Optional<double> descender;
  Optional<double> x_height;
  Optional<double> cap_height;
  Optional<double> ascender;
  Optional<double> italic_angle;

  // Miscellaneous Information
  std::string note;

  // OpenType gasp Table Fields
  std::vector<GASPRangeRecord> open_type_gasp_range_records;

  // OpenType head Table Fields
  std::string open_type_head_created;
  Optional<unsigned int> open_type_head_lowest_rec_ppem;
  std::vector<unsigned int> open_type_head_flags;

  // OpenType hhea Table Fields
  Optional<int> open_type_hhea_ascender;
  Optional<int> open_type_hhea_descender;
  Optional<int> open_type_hhea_line_gap;
  Optional<int> open_type_hhea_caret_slope_rise;
  Optional<int> open_type_hhea_caret_slope_run;
  Optional<int> open_type_hhea_caret_offset;

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
  std::vector<NameRecord> open_type_name_records;

  // OpenType OS/2 Table Fields
  Optional<int> open_type_os2_width_class;
  Optional<int> open_type_os2_weight_class;
  std::vector<unsigned int> open_type_os2_selection;
  std::string open_type_os2_vendor_id;
  std::vector<unsigned int> open_type_os2_panose;
  std::vector<unsigned int> open_type_os2_family_class;
  std::vector<unsigned int> open_type_os2_unicode_ranges;
  std::vector<unsigned int> open_type_os2_code_page_ranges;
  Optional<int> open_type_os2_typo_ascender;
  Optional<int> open_type_os2_typo_descender;
  Optional<int> open_type_os2_typo_line_gap;
  Optional<unsigned int> open_type_os2_win_ascent;
  Optional<unsigned int> open_type_os2_win_descent;
  std::vector<unsigned int> open_type_os2_type;
  Optional<int> open_type_os2_subscript_x_size;
  Optional<int> open_type_os2_subscript_y_size;
  Optional<int> open_type_os2_subscript_x_offset;
  Optional<int> open_type_os2_subscript_y_offset;
  Optional<int> open_type_os2_superscript_x_size;
  Optional<int> open_type_os2_superscript_y_size;
  Optional<int> open_type_os2_superscript_x_offset;
  Optional<int> open_type_os2_superscript_y_offset;
  Optional<int> open_type_os2_strikeout_size;
  Optional<int> open_type_os2_strikeout_position;

  // OpenType vhea Table Fields
  Optional<int> open_type_vhea_vert_typo_ascender;
  Optional<int> open_type_vhea_vert_typo_descender;
  Optional<int> open_type_vhea_vert_typo_line_gap;
  Optional<int> open_type_vhea_caret_slope_rise;
  Optional<int> open_type_vhea_caret_slope_run;
  Optional<int> open_type_vhea_caret_offset;

  // PostScript Specific Data
  std::string postscript_font_name;
  std::string postscript_full_name;
  Optional<double> postscript_slant_angle;
  Optional<int> postscript_unique_id;
  Optional<double> postscript_underline_thickness;
  Optional<double> postscript_underline_position;
  Optional<bool> postscript_is_fixed_pitch;
  std::vector<double> postscript_blue_values;
  std::vector<double> postscript_other_blues;
  std::vector<double> postscript_family_blues;
  std::vector<double> postscript_family_other_blues;
  std::vector<double> postscript_stem_snap_h;
  std::vector<double> postscript_stem_snap_v;
  Optional<double> postscript_blue_fuzz;
  Optional<double> postscript_blue_shift;
  Optional<float> postscript_blue_scale;
  Optional<bool> postscript_force_bold;
  Optional<double> postscript_default_width_x;
  Optional<double> postscript_nominal_width_x;
  std::string postscript_weight_name;
  std::string postscript_default_character;
  Optional<int> postscript_windows_character_set;

  // Macintosh FOND Resource Data
  Optional<int> macintosh_fond_family_id;
  std::string macintosh_fond_name;

  // WOFF Data
  Optional<unsigned int> woff_major_version;
  Optional<unsigned int> woff_minor_version;
  Optional<woff::metadata::UniqueID> woff_metadata_unique_id;
  Optional<woff::metadata::Vendor> woff_metadata_vendor;
  Optional<woff::metadata::Credits> woff_metadata_credits;
  Optional<woff::metadata::Description> woff_metadata_description;
  Optional<woff::metadata::License> woff_metadata_license;
  Optional<woff::metadata::Copyright> woff_metadata_copyright;
  Optional<woff::metadata::Trademark> woff_metadata_trademark;
  Optional<woff::metadata::Licensee> woff_metadata_licensee;
  std::vector<woff::metadata::Extension> woff_metadata_extensions;

  // Guidelines
  std::vector<Guideline> guidelines;
};

// MARK: -

inline FontInfo::FontInfo(const std::string& path) {
  open(path);
}

inline FontInfo::FontInfo(std::istream& stream) {
  open(stream);
}

}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_FONT_INFO_H_
