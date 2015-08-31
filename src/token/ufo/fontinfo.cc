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

extern "C" {

#include <plist/plist.h>

}  // extern "C"

#include <cassert>
#include <fstream>
#include <string>
#include <utility>

#include <boost/filesystem/path.hpp>

#include "token/ufo/plist.h"
#include "token/ufo/property_list.h"
#include "token/ufo/woff.h"

namespace token {
namespace ufo {

Fontinfo::Fontinfo(const std::string& path)
    : path_(path),
      fontinfo_(openPropertyList("fontinfo.plist")) {
  readGenericLegalInformation();
  readGenericDimensionInformation();
  readGenericMiscellaneousInformation();
  readOpenTypeGASPTableFields();
  readOpenTypeHEADTableFields();
  readOpenTypeHHEATableFields();
  readOpenTypeNameTableFields();
  readOpenTypeOS2TableFields();
  readOpenTypeVHEATableFields();
  readPostScriptSpecificData();
  readMacintoshFONDResourceData();
  readWOFFData();
  readGuidelines();
}

PropertyList Fontinfo::openPropertyList(const std::string& file) const {
  const auto path = boost::filesystem::path(path_) / file;
  std::ifstream stream(path.string());
  if (!stream.good()) {
    return PropertyList();
  }
  const std::istreambuf_iterator<char> first(stream);
  const std::string contents(first, std::istreambuf_iterator<char>());
  plist_t plist{};
  plist_from_xml(contents.c_str(), contents.size(), &plist);
  if (!plist) {
    return PropertyList();
  }
  assert(plist_get_node_type(plist) == PLIST_DICT);
  return PropertyList(plist);
}

void Fontinfo::readGenericIdentificationInformation() {
  plist::read_string(fontinfo_, "familyName", &family_name);
  plist::read_string(fontinfo_, "styleName", &style_name);
  plist::read_string(fontinfo_, "styleMapFamilyName", &style_map_family_name);
  plist::read_string(fontinfo_, "styleMapStyleName", &style_map_style_name);
  plist::read_number(fontinfo_, "versionMajor", &version_major);
  plist::read_number(fontinfo_, "versionMinor", &version_minor);
  plist::read_number(fontinfo_, "year", &year);
}

void Fontinfo::readGenericLegalInformation() {
  plist::read_string(fontinfo_, "copyright", &copyright);
  plist::read_string(fontinfo_, "trademark", &trademark);
  
}

void Fontinfo::readGenericDimensionInformation() {
  plist::read_number(fontinfo_, "unitsPerEm", &units_per_em);
  plist::read_number(fontinfo_, "descender", &descender);
  plist::read_number(fontinfo_, "xHeight", &x_height);
  plist::read_number(fontinfo_, "capHeight", &cap_height);
  plist::read_number(fontinfo_, "ascender", &ascender);
  plist::read_number(fontinfo_, "italicAngle", &italic_angle);
}

void Fontinfo::readGenericMiscellaneousInformation() {
  plist::read_string(fontinfo_, "note", &note);
}

void Fontinfo::readOpenTypeGASPTableFields() {
  plist::read_vector(fontinfo_, "openTypeGaspRangeRecords",
                     &open_type_gasp_range_records);
}

void Fontinfo::readOpenTypeHEADTableFields() {
  plist::read_string(fontinfo_, "openTypeHeadCreated",
                     &open_type_head_created);
  plist::read_number(fontinfo_, "openTypeHeadLowestRecPPEM",
                     &open_type_head_lowest_rec_ppem);
  plist::read_vector(fontinfo_, "openTypeHeadFlags",
                     &open_type_head_flags);
}

void Fontinfo::readOpenTypeHHEATableFields() {
  plist::read_number(fontinfo_, "openTypeHheaAscender",
                     &open_type_hhea_ascender);
  plist::read_number(fontinfo_, "openTypeHheaDescender",
                     &open_type_hhea_descender);
  plist::read_number(fontinfo_, "openTypeHheaLineGap",
                     &open_type_hhea_line_gap);
  plist::read_number(fontinfo_, "openTypeHheaCaretSlopeRise",
                     &open_type_hhea_caret_slope_rise);
  plist::read_number(fontinfo_, "openTypeHheaCaretSlopeRun",
                     &open_type_hhea_caret_slope_run);
  plist::read_number(fontinfo_, "openTypeHheaCaretOffset",
                     &open_type_hhea_caret_offset);
}

void Fontinfo::readOpenTypeNameTableFields() {
  plist::read_string(fontinfo_, "openTypeNameDesigner",
                     &open_type_name_designer);
  plist::read_string(fontinfo_, "openTypeNameDesignerURL",
                     &open_type_name_designer_url);
  plist::read_string(fontinfo_, "openTypeNameManufacturer",
                     &open_type_name_manufacturer);
  plist::read_string(fontinfo_, "openTypeNameManufacturerURL",
                     &open_type_name_manufacturer_url);
  plist::read_string(fontinfo_, "openTypeNameLicense",
                     &open_type_name_license);
  plist::read_string(fontinfo_, "openTypeNameLicenseURL",
                     &open_type_name_license_url);
  plist::read_string(fontinfo_, "openTypeNameVersion",
                     &open_type_name_version);
  plist::read_string(fontinfo_, "openTypeNameUniqueID",
                     &open_type_name_unique_id);
  plist::read_string(fontinfo_, "openTypeNameDescription",
                     &open_type_name_description);
  plist::read_string(fontinfo_, "openTypeNamePreferredFamilyName",
                     &open_type_name_preferred_family_name);
  plist::read_string(fontinfo_, "openTypeNamePreferredSubfamilyName",
                     &open_type_name_preferred_subfamily_name);
  plist::read_string(fontinfo_, "openTypeNameCompatibleFullName",
                     &open_type_name_compatible_full_name);
  plist::read_string(fontinfo_, "openTypeNameSampleText",
                     &open_type_name_sample_text);
  plist::read_string(fontinfo_, "openTypeNameWWSFamilyName",
                     &open_type_name_wws_family_name);
  plist::read_string(fontinfo_, "openTypeNameWWSSubfamilyName",
                     &open_type_name_wws_subfamily_name);
  plist::read_vector(fontinfo_, "openTypeNameRecords",
                     &open_type_name_records);
}

void Fontinfo::readOpenTypeOS2TableFields() {
  plist::read_number(fontinfo_, "openTypeOS2WidthClass",
                     &open_type_os2_width_class);
  plist::read_number(fontinfo_, "openTypeOS2WeightClass",
                     &open_type_os2_weight_class);
  plist::read_vector(fontinfo_, "openTypeOS2Selection",
                     &open_type_os2_selection);
  plist::read_string(fontinfo_, "openTypeOS2VendorID",
                     &open_type_os2_vendor_id);
  plist::read_vector(fontinfo_, "openTypeOS2Panose",
                     &open_type_os2_panose);
  plist::read_vector(fontinfo_, "openTypeOS2FamilyClass",
                     &open_type_os2_family_class);
  plist::read_vector(fontinfo_, "openTypeOS2UnicodeRanges",
                     &open_type_os2_unicode_ranges);
  plist::read_vector(fontinfo_, "openTypeOS2CodePageRanges",
                     &open_type_os2_code_page_ranges);
  plist::read_number(fontinfo_, "openTypeOS2TypoAscender",
                     &open_type_os2_typo_ascender);
  plist::read_number(fontinfo_, "openTypeOS2TypoDescender",
                     &open_type_os2_typo_descender);
  plist::read_number(fontinfo_, "openTypeOS2TypoLineGap",
                     &open_type_os2_typo_line_gap);
  plist::read_number(fontinfo_, "openTypeOS2WinAscent",
                     &open_type_os2_win_ascent);
  plist::read_number(fontinfo_, "openTypeOS2WinDescent",
                     &open_type_os2_win_descent);
  plist::read_vector(fontinfo_, "openTypeOS2Type",
                     &open_type_os2_type);
  plist::read_number(fontinfo_, "openTypeOS2SubscriptXSize",
                     &open_type_os2_subscript_x_size);
  plist::read_number(fontinfo_, "openTypeOS2SubscriptYSize",
                     &open_type_os2_subscript_y_size);
  plist::read_number(fontinfo_, "openTypeOS2SubscriptXOffset",
                     &open_type_os2_subscript_x_offset);
  plist::read_number(fontinfo_, "openTypeOS2SubscriptYOffset",
                     &open_type_os2_subscript_y_offset);
  plist::read_number(fontinfo_, "openTypeOS2SuperscriptXSize",
                     &open_type_os2_superscript_x_size);
  plist::read_number(fontinfo_, "openTypeOS2SuperscriptYSize",
                     &open_type_os2_superscript_y_size);
  plist::read_number(fontinfo_, "openTypeOS2SuperscriptXOffset",
                     &open_type_os2_superscript_x_offset);
  plist::read_number(fontinfo_, "openTypeOS2SuperscriptYOffset",
                     &open_type_os2_superscript_y_offset);
  plist::read_number(fontinfo_, "openTypeOS2StrikeoutSize",
                     &open_type_os2_strikeout_size);
  plist::read_number(fontinfo_, "openTypeOS2StrikeoutPosition",
                     &open_type_os2_strikeout_position);
}

void Fontinfo::readOpenTypeVHEATableFields() {
  plist::read_number(fontinfo_, "openTypeHheaAscender",
                     &open_type_vhea_vert_typo_ascender);
  plist::read_number(fontinfo_, "openTypeHheaDescender",
                     &open_type_vhea_vert_typo_descender);
  plist::read_number(fontinfo_, "openTypeHheaLineGap",
                     &open_type_vhea_vert_typo_line_gap);
  plist::read_number(fontinfo_, "openTypeHheaCaretSlopeRise",
                     &open_type_vhea_caret_slope_rise);
  plist::read_number(fontinfo_, "openTypeHheaCaretSlopeRun",
                     &open_type_vhea_caret_slope_run);
  plist::read_number(fontinfo_, "openTypeHheaCaretOffset",
                     &open_type_vhea_caret_offset);
}

void Fontinfo::readPostScriptSpecificData() {
  plist::read_string(fontinfo_, "postscriptFontName",
                     &postscript_font_name);
  plist::read_string(fontinfo_, "postscriptFullName",
                     &postscript_full_name);
  plist::read_number(fontinfo_, "postscriptSlantAngle",
                     &postscript_slant_angle);
  plist::read_number(fontinfo_, "postscriptUniqueID",
                     &postscript_unique_id);
  plist::read_number(fontinfo_, "postscriptUnderlineThickness",
                     &postscript_underline_thickness);
  plist::read_number(fontinfo_, "postscriptUnderlinePosition",
                     &postscript_underline_position);
  plist::read_number(fontinfo_, "postscriptIsFixedPitch",
                     &postscript_is_fixed_pitch);
  plist::read_vector(fontinfo_, "postscriptBlueValues",
                     &postscript_blue_values);
  plist::read_vector(fontinfo_, "postscriptOtherBlues",
                     &postscript_other_blues);
  plist::read_vector(fontinfo_, "postscriptFamilyBlues",
                     &postscript_family_blues);
  plist::read_vector(fontinfo_, "postscriptFamilyOtherBlues",
                     &postscript_family_other_blues);
  plist::read_vector(fontinfo_, "postscriptStemSnapH",
                     &postscript_stem_snap_h);
  plist::read_vector(fontinfo_, "postscriptStemSnapV",
                     &postscript_stem_snap_v);
  plist::read_number(fontinfo_, "postscriptBlueFuzz",
                     &postscript_blue_fuzz);
  plist::read_number(fontinfo_, "postscriptBlueShift",
                     &postscript_blue_shift);
  plist::read_number(fontinfo_, "postscriptBlueScale",
                     &postscript_blue_scale);
  plist::read_number(fontinfo_, "postscriptForceBold",
                     &postscript_force_bold);
  plist::read_number(fontinfo_, "postscriptDefaultWidthX",
                     &postscript_default_width_x);
  plist::read_number(fontinfo_, "postscriptNominalWidthX",
                     &postscript_nominal_width_x);
  plist::read_string(fontinfo_, "postscriptWeightName",
                     &postscript_weight_name);
  plist::read_string(fontinfo_, "postscriptDefaultCharacter",
                     &postscript_default_character);
  plist::read_number(fontinfo_, "postscriptWindowsCharacterSet",
                     &postscript_windows_character_set);
}

void Fontinfo::readMacintoshFONDResourceData() {
  plist::read_number(fontinfo_, "macintoshFONDFamilyID",
                     &macintosh_fond_family_id);
  plist::read_string(fontinfo_, "macintoshFONDName",
                     &macintosh_fond_name);
}

void Fontinfo::readWOFFData() {
  plist::read_number(fontinfo_, "woffMajorVersion", &woff_major_version);
  plist::read_number(fontinfo_, "woffMinorVersion", &woff_minor_version);
  plist::read_object(fontinfo_, "woffMetadataUniqueID",
                     &woff_metadata_unique_id);
  plist::read_object(fontinfo_, "woffMetadataVendor",
                     &woff_metadata_vendor);
  plist::read_object(fontinfo_, "woffMetadataCredits",
                     &woff_metadata_credits);
  plist::read_object(fontinfo_, "woffMetadataDescription",
                     &woff_metadata_description);
  plist::read_object(fontinfo_, "woffMetadataLicense",
                     &woff_metadata_license);
  plist::read_object(fontinfo_, "woffMetadataCopyright",
                     &woff_metadata_copyright);
  plist::read_object(fontinfo_, "woffMetadataTrademark",
                     &woff_metadata_trademark);
  plist::read_object(fontinfo_, "woffMetadataLicensee",
                     &woff_metadata_licensee);
  plist::read_vector(fontinfo_, "woffMetadataExtensions",
                     &woff_metadata_extensions);
}

void Fontinfo::readGuidelines() {
  plist::read_vector(fontinfo_, "guidelines", &guidelines);
}

}  // namespace ufo
}  // namespace token
