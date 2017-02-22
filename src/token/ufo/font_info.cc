//
//  token/ufo/font_info.cc
//
//  MIT License
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

#include "token/ufo/font_info.h"

extern "C" {

#include <plist/plist.h>

}  // extern "C"

#include <cassert>
#include <cstdlib>
#include <fstream>
#include <istream>
#include <ostream>
#include <string>
#include <utility>

#include <boost/filesystem/path.hpp>

#include "token/ufo/plist.h"
#include "token/ufo/property_list.h"
#include "token/ufo/woff.h"

namespace token {
namespace ufo {

// MARK: Opening and saving

bool FontInfo::open(const std::string& path) {
  const boost::filesystem::path node(path);
  std::string font_info = path;
  if (node.leaf().extension() == ".ufo") {
    font_info = (node / "fontinfo.plist").string();
  }
  std::ifstream stream(font_info);
  const auto result = open(&stream);
  stream.close();
  return result;
}

bool FontInfo::open(std::istream *stream) {
  assert(stream);
  PropertyList plist(stream);
  readIdentificationInformation(plist);
  readLegalInformation(plist);
  readDimensionInformation(plist);
  readMiscellaneousInformation(plist);
  readOpenTypeGASPTableFields(plist);
  readOpenTypeHEADTableFields(plist);
  readOpenTypeHHEATableFields(plist);
  readOpenTypeNameTableFields(plist);
  readOpenTypeOS2TableFields(plist);
  readOpenTypeVHEATableFields(plist);
  readPostScriptSpecificData(plist);
  readMacintoshFONDResourceData(plist);
  readWOFFData(plist);
  readGuidelines(plist);
  return true;
}

bool FontInfo::save(const std::string& path) const {
  const boost::filesystem::path node(path);
  std::string font_info = path;
  if (node.leaf().extension() == ".ufo") {
    font_info = (node / "fontinfo.plist").string();
  }
  std::ofstream stream(font_info);
  const auto result = save(&stream);
  stream.close();
  return result;
}

bool FontInfo::save(std::ostream *stream) const {
  assert(stream);
  PropertyList plist;
  writeIdentificationInformation(plist);
  writeLegalInformation(plist);
  writeDimensionInformation(plist);
  writeMiscellaneousInformation(plist);
  writeOpenTypeGASPTableFields(plist);
  writeOpenTypeHEADTableFields(plist);
  writeOpenTypeHHEATableFields(plist);
  writeOpenTypeNameTableFields(plist);
  writeOpenTypeOS2TableFields(plist);
  writeOpenTypeVHEATableFields(plist);
  writePostScriptSpecificData(plist);
  writeMacintoshFONDResourceData(plist);
  writeWOFFData(plist);
  writeGuidelines(plist);
  plist.save(stream);
  return true;
}

// MARK: Reading from property list

void FontInfo::readIdentificationInformation(const PropertyList& plist) {
  plist::read_string(plist, "familyName", &family_name);
  plist::read_string(plist, "styleName", &style_name);
  plist::read_string(plist, "styleMapFamilyName", &style_map_family_name);
  plist::read_string(plist, "styleMapStyleName", &style_map_style_name);
  plist::read_number(plist, "versionMajor", &version_major);
  plist::read_number(plist, "versionMinor", &version_minor);
  plist::read_number(plist, "year", &year);
}

void FontInfo::readLegalInformation(const PropertyList& plist) {
  plist::read_string(plist, "copyright", &copyright);
  plist::read_string(plist, "trademark", &trademark);
}

void FontInfo::readDimensionInformation(const PropertyList& plist) {
  plist::read_number(plist, "unitsPerEm", &units_per_em);
  plist::read_number(plist, "descender", &descender);
  plist::read_number(plist, "xHeight", &x_height);
  plist::read_number(plist, "capHeight", &cap_height);
  plist::read_number(plist, "ascender", &ascender);
  plist::read_number(plist, "italicAngle", &italic_angle);
}

void FontInfo::readMiscellaneousInformation(const PropertyList& plist) {
  plist::read_string(plist, "note", &note);
}

void FontInfo::readOpenTypeGASPTableFields(const PropertyList& plist) {
  plist::read_vector(plist, "openTypeGaspRangeRecords",
                     &open_type_gasp_range_records);
}

void FontInfo::readOpenTypeHEADTableFields(const PropertyList& plist) {
  plist::read_string(plist, "openTypeHeadCreated",
                     &open_type_head_created);
  plist::read_number(plist, "openTypeHeadLowestRecPPEM",
                     &open_type_head_lowest_rec_ppem);
  plist::read_vector(plist, "openTypeHeadFlags",
                     &open_type_head_flags);
}

void FontInfo::readOpenTypeHHEATableFields(const PropertyList& plist) {
  plist::read_number(plist, "openTypeHheaAscender",
                     &open_type_hhea_ascender);
  plist::read_number(plist, "openTypeHheaDescender",
                     &open_type_hhea_descender);
  plist::read_number(plist, "openTypeHheaLineGap",
                     &open_type_hhea_line_gap);
  plist::read_number(plist, "openTypeHheaCaretSlopeRise",
                     &open_type_hhea_caret_slope_rise);
  plist::read_number(plist, "openTypeHheaCaretSlopeRun",
                     &open_type_hhea_caret_slope_run);
  plist::read_number(plist, "openTypeHheaCaretOffset",
                     &open_type_hhea_caret_offset);
}

void FontInfo::readOpenTypeNameTableFields(const PropertyList& plist) {
  plist::read_string(plist, "openTypeNameDesigner",
                     &open_type_name_designer);
  plist::read_string(plist, "openTypeNameDesignerURL",
                     &open_type_name_designer_url);
  plist::read_string(plist, "openTypeNameManufacturer",
                     &open_type_name_manufacturer);
  plist::read_string(plist, "openTypeNameManufacturerURL",
                     &open_type_name_manufacturer_url);
  plist::read_string(plist, "openTypeNameLicense",
                     &open_type_name_license);
  plist::read_string(plist, "openTypeNameLicenseURL",
                     &open_type_name_license_url);
  plist::read_string(plist, "openTypeNameVersion",
                     &open_type_name_version);
  plist::read_string(plist, "openTypeNameUniqueID",
                     &open_type_name_unique_id);
  plist::read_string(plist, "openTypeNameDescription",
                     &open_type_name_description);
  plist::read_string(plist, "openTypeNamePreferredFamilyName",
                     &open_type_name_preferred_family_name);
  plist::read_string(plist, "openTypeNamePreferredSubfamilyName",
                     &open_type_name_preferred_subfamily_name);
  plist::read_string(plist, "openTypeNameCompatibleFullName",
                     &open_type_name_compatible_full_name);
  plist::read_string(plist, "openTypeNameSampleText",
                     &open_type_name_sample_text);
  plist::read_string(plist, "openTypeNameWWSFamilyName",
                     &open_type_name_wws_family_name);
  plist::read_string(plist, "openTypeNameWWSSubfamilyName",
                     &open_type_name_wws_subfamily_name);
  plist::read_vector(plist, "openTypeNameRecords",
                     &open_type_name_records);
}

void FontInfo::readOpenTypeOS2TableFields(const PropertyList& plist) {
  plist::read_number(plist, "openTypeOS2WidthClass",
                     &open_type_os2_width_class);
  plist::read_number(plist, "openTypeOS2WeightClass",
                     &open_type_os2_weight_class);
  plist::read_vector(plist, "openTypeOS2Selection",
                     &open_type_os2_selection);
  plist::read_string(plist, "openTypeOS2VendorID",
                     &open_type_os2_vendor_id);
  plist::read_vector(plist, "openTypeOS2Panose",
                     &open_type_os2_panose);
  plist::read_vector(plist, "openTypeOS2FamilyClass",
                     &open_type_os2_family_class);
  plist::read_vector(plist, "openTypeOS2UnicodeRanges",
                     &open_type_os2_unicode_ranges);
  plist::read_vector(plist, "openTypeOS2CodePageRanges",
                     &open_type_os2_code_page_ranges);
  plist::read_number(plist, "openTypeOS2TypoAscender",
                     &open_type_os2_typo_ascender);
  plist::read_number(plist, "openTypeOS2TypoDescender",
                     &open_type_os2_typo_descender);
  plist::read_number(plist, "openTypeOS2TypoLineGap",
                     &open_type_os2_typo_line_gap);
  plist::read_number(plist, "openTypeOS2WinAscent",
                     &open_type_os2_win_ascent);
  plist::read_number(plist, "openTypeOS2WinDescent",
                     &open_type_os2_win_descent);
  plist::read_vector(plist, "openTypeOS2Type",
                     &open_type_os2_type);
  plist::read_number(plist, "openTypeOS2SubscriptXSize",
                     &open_type_os2_subscript_x_size);
  plist::read_number(plist, "openTypeOS2SubscriptYSize",
                     &open_type_os2_subscript_y_size);
  plist::read_number(plist, "openTypeOS2SubscriptXOffset",
                     &open_type_os2_subscript_x_offset);
  plist::read_number(plist, "openTypeOS2SubscriptYOffset",
                     &open_type_os2_subscript_y_offset);
  plist::read_number(plist, "openTypeOS2SuperscriptXSize",
                     &open_type_os2_superscript_x_size);
  plist::read_number(plist, "openTypeOS2SuperscriptYSize",
                     &open_type_os2_superscript_y_size);
  plist::read_number(plist, "openTypeOS2SuperscriptXOffset",
                     &open_type_os2_superscript_x_offset);
  plist::read_number(plist, "openTypeOS2SuperscriptYOffset",
                     &open_type_os2_superscript_y_offset);
  plist::read_number(plist, "openTypeOS2StrikeoutSize",
                     &open_type_os2_strikeout_size);
  plist::read_number(plist, "openTypeOS2StrikeoutPosition",
                     &open_type_os2_strikeout_position);
}

void FontInfo::readOpenTypeVHEATableFields(const PropertyList& plist) {
  plist::read_number(plist, "openTypeVheaVertTypoAscender",
                     &open_type_vhea_vert_typo_ascender);
  plist::read_number(plist, "openTypeVheaVertTypoDescender",
                     &open_type_vhea_vert_typo_descender);
  plist::read_number(plist, "openTypeVheaVertTypoLineGap",
                     &open_type_vhea_vert_typo_line_gap);
  plist::read_number(plist, "openTypeVheaCaretSlopeRise",
                     &open_type_vhea_caret_slope_rise);
  plist::read_number(plist, "openTypeVheaCaretSlopeRun",
                     &open_type_vhea_caret_slope_run);
  plist::read_number(plist, "openTypeVheaCaretOffset",
                     &open_type_vhea_caret_offset);
}

void FontInfo::readPostScriptSpecificData(const PropertyList& plist) {
  plist::read_string(plist, "postscriptFontName",
                     &postscript_font_name);
  plist::read_string(plist, "postscriptFullName",
                     &postscript_full_name);
  plist::read_number(plist, "postscriptSlantAngle",
                     &postscript_slant_angle);
  plist::read_number(plist, "postscriptUniqueID",
                     &postscript_unique_id);
  plist::read_number(plist, "postscriptUnderlineThickness",
                     &postscript_underline_thickness);
  plist::read_number(plist, "postscriptUnderlinePosition",
                     &postscript_underline_position);
  plist::read_boolean(plist, "postscriptIsFixedPitch",
                      &postscript_is_fixed_pitch);
  plist::read_vector(plist, "postscriptBlueValues",
                     &postscript_blue_values);
  plist::read_vector(plist, "postscriptOtherBlues",
                     &postscript_other_blues);
  plist::read_vector(plist, "postscriptFamilyBlues",
                     &postscript_family_blues);
  plist::read_vector(plist, "postscriptFamilyOtherBlues",
                     &postscript_family_other_blues);
  plist::read_vector(plist, "postscriptStemSnapH",
                     &postscript_stem_snap_h);
  plist::read_vector(plist, "postscriptStemSnapV",
                     &postscript_stem_snap_v);
  plist::read_number(plist, "postscriptBlueFuzz",
                     &postscript_blue_fuzz);
  plist::read_number(plist, "postscriptBlueShift",
                     &postscript_blue_shift);
  plist::read_number(plist, "postscriptBlueScale",
                     &postscript_blue_scale);
  plist::read_boolean(plist, "postscriptForceBold",
                      &postscript_force_bold);
  plist::read_number(plist, "postscriptDefaultWidthX",
                     &postscript_default_width_x);
  plist::read_number(plist, "postscriptNominalWidthX",
                     &postscript_nominal_width_x);
  plist::read_string(plist, "postscriptWeightName",
                     &postscript_weight_name);
  plist::read_string(plist, "postscriptDefaultCharacter",
                     &postscript_default_character);
  plist::read_number(plist, "postscriptWindowsCharacterSet",
                     &postscript_windows_character_set);
}

void FontInfo::readMacintoshFONDResourceData(const PropertyList& plist) {
  plist::read_number(plist, "macintoshFONDFamilyID", &macintosh_fond_family_id);
  plist::read_string(plist, "macintoshFONDName", &macintosh_fond_name);
}

void FontInfo::readWOFFData(const PropertyList& plist) {
  plist::read_number(plist, "woffMajorVersion", &woff_major_version);
  plist::read_number(plist, "woffMinorVersion", &woff_minor_version);
  plist::read_object(plist, "woffMetadataUniqueID",
                     &woff_metadata_unique_id);
  plist::read_object(plist, "woffMetadataVendor",
                     &woff_metadata_vendor);
  plist::read_object(plist, "woffMetadataCredits",
                     &woff_metadata_credits);
  plist::read_object(plist, "woffMetadataDescription",
                     &woff_metadata_description);
  plist::read_object(plist, "woffMetadataLicense",
                     &woff_metadata_license);
  plist::read_object(plist, "woffMetadataCopyright",
                     &woff_metadata_copyright);
  plist::read_object(plist, "woffMetadataTrademark",
                     &woff_metadata_trademark);
  plist::read_object(plist, "woffMetadataLicensee",
                     &woff_metadata_licensee);
  plist::read_vector(plist, "woffMetadataExtensions",
                     &woff_metadata_extensions);
}

void FontInfo::readGuidelines(const PropertyList& plist) {
  plist::read_vector(plist, "guidelines", &guidelines);
}

// MARK: Writing to property list

void FontInfo::writeIdentificationInformation(const PropertyList& plist) const {
  plist::write_string(plist, "familyName", family_name);
  plist::write_string(plist, "styleName", style_name);
  plist::write_string(plist, "styleMapFamilyName", style_map_family_name);
  plist::write_string(plist, "styleMapStyleName", style_map_style_name);
  plist::write_number(plist, "versionMajor", version_major);
  plist::write_number(plist, "versionMinor", version_minor);
  plist::write_number(plist, "year", year);
}

void FontInfo::writeLegalInformation(const PropertyList& plist) const {
  plist::write_string(plist, "copyright", copyright);
  plist::write_string(plist, "trademark", trademark);
}

void FontInfo::writeDimensionInformation(const PropertyList& plist) const {
  plist::write_number(plist, "unitsPerEm", units_per_em);
  plist::write_number(plist, "descender", descender);
  plist::write_number(plist, "xHeight", x_height);
  plist::write_number(plist, "capHeight", cap_height);
  plist::write_number(plist, "ascender", ascender);
  plist::write_number(plist, "italicAngle", italic_angle);
}

void FontInfo::writeMiscellaneousInformation(const PropertyList& plist) const {
  plist::write_string(plist, "note", note);
}

void FontInfo::writeOpenTypeGASPTableFields(const PropertyList& plist) const {
  plist::write_vector(plist, "openTypeGaspRangeRecords",
                      open_type_gasp_range_records);
}

void FontInfo::writeOpenTypeHEADTableFields(const PropertyList& plist) const {
  plist::write_string(plist, "openTypeHeadCreated",
                      open_type_head_created);
  plist::write_number(plist, "openTypeHeadLowestRecPPEM",
                      open_type_head_lowest_rec_ppem);
  plist::write_vector(plist, "openTypeHeadFlags",
                      open_type_head_flags);
}

void FontInfo::writeOpenTypeHHEATableFields(const PropertyList& plist) const {
  plist::write_number(plist, "openTypeHheaAscender",
                      open_type_hhea_ascender);
  plist::write_number(plist, "openTypeHheaDescender",
                      open_type_hhea_descender);
  plist::write_number(plist, "openTypeHheaLineGap",
                      open_type_hhea_line_gap);
  plist::write_number(plist, "openTypeHheaCaretSlopeRise",
                      open_type_hhea_caret_slope_rise);
  plist::write_number(plist, "openTypeHheaCaretSlopeRun",
                      open_type_hhea_caret_slope_run);
  plist::write_number(plist, "openTypeHheaCaretOffset",
                      open_type_hhea_caret_offset);
}

void FontInfo::writeOpenTypeNameTableFields(const PropertyList& plist) const {
  plist::write_string(plist, "openTypeNameDesigner",
                      open_type_name_designer);
  plist::write_string(plist, "openTypeNameDesignerURL",
                      open_type_name_designer_url);
  plist::write_string(plist, "openTypeNameManufacturer",
                      open_type_name_manufacturer);
  plist::write_string(plist, "openTypeNameManufacturerURL",
                      open_type_name_manufacturer_url);
  plist::write_string(plist, "openTypeNameLicense",
                      open_type_name_license);
  plist::write_string(plist, "openTypeNameLicenseURL",
                      open_type_name_license_url);
  plist::write_string(plist, "openTypeNameVersion",
                      open_type_name_version);
  plist::write_string(plist, "openTypeNameUniqueID",
                      open_type_name_unique_id);
  plist::write_string(plist, "openTypeNameDescription",
                      open_type_name_description);
  plist::write_string(plist, "openTypeNamePreferredFamilyName",
                      open_type_name_preferred_family_name);
  plist::write_string(plist, "openTypeNamePreferredSubfamilyName",
                      open_type_name_preferred_subfamily_name);
  plist::write_string(plist, "openTypeNameCompatibleFullName",
                      open_type_name_compatible_full_name);
  plist::write_string(plist, "openTypeNameSampleText",
                      open_type_name_sample_text);
  plist::write_string(plist, "openTypeNameWWSFamilyName",
                      open_type_name_wws_family_name);
  plist::write_string(plist, "openTypeNameWWSSubfamilyName",
                      open_type_name_wws_subfamily_name);
  plist::write_vector(plist, "openTypeNameRecords",
                      open_type_name_records);
}

void FontInfo::writeOpenTypeOS2TableFields(const PropertyList& plist) const {
  plist::write_number(plist, "openTypeOS2WidthClass",
                      open_type_os2_width_class);
  plist::write_number(plist, "openTypeOS2WeightClass",
                      open_type_os2_weight_class);
  plist::write_vector(plist, "openTypeOS2Selection",
                      open_type_os2_selection);
  plist::write_string(plist, "openTypeOS2VendorID",
                      open_type_os2_vendor_id);
  plist::write_vector(plist, "openTypeOS2Panose",
                      open_type_os2_panose);
  plist::write_vector(plist, "openTypeOS2FamilyClass",
                      open_type_os2_family_class);
  plist::write_vector(plist, "openTypeOS2UnicodeRanges",
                      open_type_os2_unicode_ranges);
  plist::write_vector(plist, "openTypeOS2CodePageRanges",
                      open_type_os2_code_page_ranges);
  plist::write_number(plist, "openTypeOS2TypoAscender",
                      open_type_os2_typo_ascender);
  plist::write_number(plist, "openTypeOS2TypoDescender",
                      open_type_os2_typo_descender);
  plist::write_number(plist, "openTypeOS2TypoLineGap",
                      open_type_os2_typo_line_gap);
  plist::write_number(plist, "openTypeOS2WinAscent",
                      open_type_os2_win_ascent);
  plist::write_number(plist, "openTypeOS2WinDescent",
                      open_type_os2_win_descent);
  plist::write_vector(plist, "openTypeOS2Type",
                      open_type_os2_type);
  plist::write_number(plist, "openTypeOS2SubscriptXSize",
                      open_type_os2_subscript_x_size);
  plist::write_number(plist, "openTypeOS2SubscriptYSize",
                      open_type_os2_subscript_y_size);
  plist::write_number(plist, "openTypeOS2SubscriptXOffset",
                      open_type_os2_subscript_x_offset);
  plist::write_number(plist, "openTypeOS2SubscriptYOffset",
                      open_type_os2_subscript_y_offset);
  plist::write_number(plist, "openTypeOS2SuperscriptXSize",
                      open_type_os2_superscript_x_size);
  plist::write_number(plist, "openTypeOS2SuperscriptYSize",
                      open_type_os2_superscript_y_size);
  plist::write_number(plist, "openTypeOS2SuperscriptXOffset",
                      open_type_os2_superscript_x_offset);
  plist::write_number(plist, "openTypeOS2SuperscriptYOffset",
                      open_type_os2_superscript_y_offset);
  plist::write_number(plist, "openTypeOS2StrikeoutSize",
                      open_type_os2_strikeout_size);
  plist::write_number(plist, "openTypeOS2StrikeoutPosition",
                      open_type_os2_strikeout_position);
}

void FontInfo::writeOpenTypeVHEATableFields(const PropertyList& plist) const {
  plist::write_number(plist, "openTypeVheaVertTypoAscender",
                      open_type_vhea_vert_typo_ascender);
  plist::write_number(plist, "openTypeVheaVertTypoDescender",
                      open_type_vhea_vert_typo_descender);
  plist::write_number(plist, "openTypeVheaVertTypoLineGap",
                      open_type_vhea_vert_typo_line_gap);
  plist::write_number(plist, "openTypeVheaCaretSlopeRise",
                      open_type_vhea_caret_slope_rise);
  plist::write_number(plist, "openTypeVheaCaretSlopeRun",
                      open_type_vhea_caret_slope_run);
  plist::write_number(plist, "openTypeVheaCaretOffset",
                      open_type_vhea_caret_offset);
}

void FontInfo::writePostScriptSpecificData(const PropertyList& plist) const {
  plist::write_string(plist, "postscriptFontName",
                      postscript_font_name);
  plist::write_string(plist, "postscriptFullName",
                      postscript_full_name);
  plist::write_number(plist, "postscriptSlantAngle",
                      postscript_slant_angle);
  plist::write_number(plist, "postscriptUniqueID",
                      postscript_unique_id);
  plist::write_number(plist, "postscriptUnderlineThickness",
                      postscript_underline_thickness);
  plist::write_number(plist, "postscriptUnderlinePosition",
                      postscript_underline_position);
  plist::write_boolean(plist, "postscriptIsFixedPitch",
                       postscript_is_fixed_pitch);
  plist::write_vector(plist, "postscriptBlueValues",
                      postscript_blue_values);
  plist::write_vector(plist, "postscriptOtherBlues",
                      postscript_other_blues);
  plist::write_vector(plist, "postscriptFamilyBlues",
                      postscript_family_blues);
  plist::write_vector(plist, "postscriptFamilyOtherBlues",
                      postscript_family_other_blues);
  plist::write_vector(plist, "postscriptStemSnapH",
                      postscript_stem_snap_h);
  plist::write_vector(plist, "postscriptStemSnapV",
                      postscript_stem_snap_v);
  plist::write_number(plist, "postscriptBlueFuzz",
                      postscript_blue_fuzz);
  plist::write_number(plist, "postscriptBlueShift",
                      postscript_blue_shift);
  plist::write_number(plist, "postscriptBlueScale",
                      postscript_blue_scale);
  plist::write_boolean(plist, "postscriptForceBold",
                       postscript_force_bold);
  plist::write_number(plist, "postscriptDefaultWidthX",
                      postscript_default_width_x);
  plist::write_number(plist, "postscriptNominalWidthX",
                      postscript_nominal_width_x);
  plist::write_string(plist, "postscriptWeightName",
                      postscript_weight_name);
  plist::write_string(plist, "postscriptDefaultCharacter",
                      postscript_default_character);
  plist::write_number(plist, "postscriptWindowsCharacterSet",
                      postscript_windows_character_set);
}

void FontInfo::writeMacintoshFONDResourceData(const PropertyList& plist) const {
  plist::write_number(plist, "macintoshFONDFamilyID", macintosh_fond_family_id);
  plist::write_string(plist, "macintoshFONDName", macintosh_fond_name);
}

void FontInfo::writeWOFFData(const PropertyList& plist) const {
  plist::write_number(plist, "woffMajorVersion", woff_major_version);
  plist::write_number(plist, "woffMinorVersion", woff_minor_version);
  plist::write_object(plist, "woffMetadataUniqueID",
                      woff_metadata_unique_id);
  plist::write_object(plist, "woffMetadataVendor",
                      woff_metadata_vendor);
  plist::write_object(plist, "woffMetadataCredits",
                      woff_metadata_credits);
  plist::write_object(plist, "woffMetadataDescription",
                      woff_metadata_description);
  plist::write_object(plist, "woffMetadataLicense",
                      woff_metadata_license);
  plist::write_object(plist, "woffMetadataCopyright",
                      woff_metadata_copyright);
  plist::write_object(plist, "woffMetadataTrademark",
                      woff_metadata_trademark);
  plist::write_object(plist, "woffMetadataLicensee",
                      woff_metadata_licensee);
  plist::write_vector(plist, "woffMetadataExtensions",
                      woff_metadata_extensions);
}

void FontInfo::writeGuidelines(const PropertyList& plist) const {
  plist::write_vector(plist, "guidelines", guidelines);
}

}  // namespace ufo
}  // namespace token
