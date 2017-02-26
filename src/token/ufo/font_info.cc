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

#include "token/ufo/font_info.h"

extern "C" {

#include <plist/plist.h>

}  // extern "C"

#include <cstdlib>
#include <fstream>
#include <istream>
#include <ostream>
#include <string>

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
  const auto result = open(stream);
  stream.close();
  return result;
}

bool FontInfo::open(std::istream& stream) {
  if (!stream.good()) {
    return false;
  }
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
  const auto result = save(stream);
  stream.close();
  return result;
}

bool FontInfo::save(std::ostream& stream) const {
  if (!stream.good()) {
    return false;
  }
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
  plist::readString(plist, "familyName", &family_name);
  plist::readString(plist, "styleName", &style_name);
  plist::readString(plist, "styleMapFamilyName", &style_map_family_name);
  plist::readString(plist, "styleMapStyleName", &style_map_style_name);
  plist::readNumber(plist, "versionMajor", &version_major);
  plist::readNumber(plist, "versionMinor", &version_minor);
  plist::readNumber(plist, "year", &year);
}

void FontInfo::readLegalInformation(const PropertyList& plist) {
  plist::readString(plist, "copyright", &copyright);
  plist::readString(plist, "trademark", &trademark);
}

void FontInfo::readDimensionInformation(const PropertyList& plist) {
  plist::readNumber(plist, "unitsPerEm", &units_per_em);
  plist::readNumber(plist, "descender", &descender);
  plist::readNumber(plist, "xHeight", &x_height);
  plist::readNumber(plist, "capHeight", &cap_height);
  plist::readNumber(plist, "ascender", &ascender);
  plist::readNumber(plist, "italicAngle", &italic_angle);
}

void FontInfo::readMiscellaneousInformation(const PropertyList& plist) {
  plist::readString(plist, "note", &note);
}

void FontInfo::readOpenTypeGASPTableFields(const PropertyList& plist) {
  plist::readVector(plist, "openTypeGaspRangeRecords",
                    &open_type_gasp_range_records);
}

void FontInfo::readOpenTypeHEADTableFields(const PropertyList& plist) {
  plist::readString(plist, "openTypeHeadCreated",
                    &open_type_head_created);
  plist::readNumber(plist, "openTypeHeadLowestRecPPEM",
                    &open_type_head_lowest_rec_ppem);
  plist::readVector(plist, "openTypeHeadFlags",
                    &open_type_head_flags);
}

void FontInfo::readOpenTypeHHEATableFields(const PropertyList& plist) {
  plist::readNumber(plist, "openTypeHheaAscender",
                    &open_type_hhea_ascender);
  plist::readNumber(plist, "openTypeHheaDescender",
                    &open_type_hhea_descender);
  plist::readNumber(plist, "openTypeHheaLineGap",
                    &open_type_hhea_line_gap);
  plist::readNumber(plist, "openTypeHheaCaretSlopeRise",
                    &open_type_hhea_caret_slope_rise);
  plist::readNumber(plist, "openTypeHheaCaretSlopeRun",
                    &open_type_hhea_caret_slope_run);
  plist::readNumber(plist, "openTypeHheaCaretOffset",
                    &open_type_hhea_caret_offset);
}

void FontInfo::readOpenTypeNameTableFields(const PropertyList& plist) {
  plist::readString(plist, "openTypeNameDesigner",
                    &open_type_name_designer);
  plist::readString(plist, "openTypeNameDesignerURL",
                    &open_type_name_designer_url);
  plist::readString(plist, "openTypeNameManufacturer",
                    &open_type_name_manufacturer);
  plist::readString(plist, "openTypeNameManufacturerURL",
                    &open_type_name_manufacturer_url);
  plist::readString(plist, "openTypeNameLicense",
                    &open_type_name_license);
  plist::readString(plist, "openTypeNameLicenseURL",
                    &open_type_name_license_url);
  plist::readString(plist, "openTypeNameVersion",
                    &open_type_name_version);
  plist::readString(plist, "openTypeNameUniqueID",
                    &open_type_name_unique_id);
  plist::readString(plist, "openTypeNameDescription",
                    &open_type_name_description);
  plist::readString(plist, "openTypeNamePreferredFamilyName",
                    &open_type_name_preferred_family_name);
  plist::readString(plist, "openTypeNamePreferredSubfamilyName",
                    &open_type_name_preferred_subfamily_name);
  plist::readString(plist, "openTypeNameCompatibleFullName",
                    &open_type_name_compatible_full_name);
  plist::readString(plist, "openTypeNameSampleText",
                    &open_type_name_sample_text);
  plist::readString(plist, "openTypeNameWWSFamilyName",
                    &open_type_name_wws_family_name);
  plist::readString(plist, "openTypeNameWWSSubfamilyName",
                    &open_type_name_wws_subfamily_name);
  plist::readVector(plist, "openTypeNameRecords",
                    &open_type_name_records);
}

void FontInfo::readOpenTypeOS2TableFields(const PropertyList& plist) {
  plist::readNumber(plist, "openTypeOS2WidthClass",
                    &open_type_os2_width_class);
  plist::readNumber(plist, "openTypeOS2WeightClass",
                    &open_type_os2_weight_class);
  plist::readVector(plist, "openTypeOS2Selection",
                    &open_type_os2_selection);
  plist::readString(plist, "openTypeOS2VendorID",
                    &open_type_os2_vendor_id);
  plist::readVector(plist, "openTypeOS2Panose",
                    &open_type_os2_panose);
  plist::readVector(plist, "openTypeOS2FamilyClass",
                    &open_type_os2_family_class);
  plist::readVector(plist, "openTypeOS2UnicodeRanges",
                    &open_type_os2_unicode_ranges);
  plist::readVector(plist, "openTypeOS2CodePageRanges",
                    &open_type_os2_code_page_ranges);
  plist::readNumber(plist, "openTypeOS2TypoAscender",
                    &open_type_os2_typo_ascender);
  plist::readNumber(plist, "openTypeOS2TypoDescender",
                    &open_type_os2_typo_descender);
  plist::readNumber(plist, "openTypeOS2TypoLineGap",
                    &open_type_os2_typo_line_gap);
  plist::readNumber(plist, "openTypeOS2WinAscent",
                    &open_type_os2_win_ascent);
  plist::readNumber(plist, "openTypeOS2WinDescent",
                    &open_type_os2_win_descent);
  plist::readVector(plist, "openTypeOS2Type",
                    &open_type_os2_type);
  plist::readNumber(plist, "openTypeOS2SubscriptXSize",
                    &open_type_os2_subscript_x_size);
  plist::readNumber(plist, "openTypeOS2SubscriptYSize",
                    &open_type_os2_subscript_y_size);
  plist::readNumber(plist, "openTypeOS2SubscriptXOffset",
                    &open_type_os2_subscript_x_offset);
  plist::readNumber(plist, "openTypeOS2SubscriptYOffset",
                    &open_type_os2_subscript_y_offset);
  plist::readNumber(plist, "openTypeOS2SuperscriptXSize",
                    &open_type_os2_superscript_x_size);
  plist::readNumber(plist, "openTypeOS2SuperscriptYSize",
                    &open_type_os2_superscript_y_size);
  plist::readNumber(plist, "openTypeOS2SuperscriptXOffset",
                    &open_type_os2_superscript_x_offset);
  plist::readNumber(plist, "openTypeOS2SuperscriptYOffset",
                    &open_type_os2_superscript_y_offset);
  plist::readNumber(plist, "openTypeOS2StrikeoutSize",
                    &open_type_os2_strikeout_size);
  plist::readNumber(plist, "openTypeOS2StrikeoutPosition",
                    &open_type_os2_strikeout_position);
}

void FontInfo::readOpenTypeVHEATableFields(const PropertyList& plist) {
  plist::readNumber(plist, "openTypeVheaVertTypoAscender",
                    &open_type_vhea_vert_typo_ascender);
  plist::readNumber(plist, "openTypeVheaVertTypoDescender",
                    &open_type_vhea_vert_typo_descender);
  plist::readNumber(plist, "openTypeVheaVertTypoLineGap",
                    &open_type_vhea_vert_typo_line_gap);
  plist::readNumber(plist, "openTypeVheaCaretSlopeRise",
                    &open_type_vhea_caret_slope_rise);
  plist::readNumber(plist, "openTypeVheaCaretSlopeRun",
                    &open_type_vhea_caret_slope_run);
  plist::readNumber(plist, "openTypeVheaCaretOffset",
                    &open_type_vhea_caret_offset);
}

void FontInfo::readPostScriptSpecificData(const PropertyList& plist) {
  plist::readString(plist, "postscriptFontName",
                    &postscript_font_name);
  plist::readString(plist, "postscriptFullName",
                    &postscript_full_name);
  plist::readNumber(plist, "postscriptSlantAngle",
                    &postscript_slant_angle);
  plist::readNumber(plist, "postscriptUniqueID",
                    &postscript_unique_id);
  plist::readNumber(plist, "postscriptUnderlineThickness",
                    &postscript_underline_thickness);
  plist::readNumber(plist, "postscriptUnderlinePosition",
                    &postscript_underline_position);
  plist::readBoolean(plist, "postscriptIsFixedPitch",
                     &postscript_is_fixed_pitch);
  plist::readVector(plist, "postscriptBlueValues",
                    &postscript_blue_values);
  plist::readVector(plist, "postscriptOtherBlues",
                    &postscript_other_blues);
  plist::readVector(plist, "postscriptFamilyBlues",
                    &postscript_family_blues);
  plist::readVector(plist, "postscriptFamilyOtherBlues",
                    &postscript_family_other_blues);
  plist::readVector(plist, "postscriptStemSnapH",
                    &postscript_stem_snap_h);
  plist::readVector(plist, "postscriptStemSnapV",
                    &postscript_stem_snap_v);
  plist::readNumber(plist, "postscriptBlueFuzz",
                    &postscript_blue_fuzz);
  plist::readNumber(plist, "postscriptBlueShift",
                    &postscript_blue_shift);
  plist::readNumber(plist, "postscriptBlueScale",
                    &postscript_blue_scale);
  plist::readBoolean(plist, "postscriptForceBold",
                     &postscript_force_bold);
  plist::readNumber(plist, "postscriptDefaultWidthX",
                    &postscript_default_width_x);
  plist::readNumber(plist, "postscriptNominalWidthX",
                    &postscript_nominal_width_x);
  plist::readString(plist, "postscriptWeightName",
                    &postscript_weight_name);
  plist::readString(plist, "postscriptDefaultCharacter",
                    &postscript_default_character);
  plist::readNumber(plist, "postscriptWindowsCharacterSet",
                    &postscript_windows_character_set);
}

void FontInfo::readMacintoshFONDResourceData(const PropertyList& plist) {
  plist::readNumber(plist, "macintoshFONDFamilyID",
                    &macintosh_fond_family_id);
  plist::readString(plist, "macintoshFONDName",
                    &macintosh_fond_name);
}

void FontInfo::readWOFFData(const PropertyList& plist) {
  plist::readNumber(plist, "woffMajorVersion",
                    &woff_major_version);
  plist::readNumber(plist, "woffMinorVersion",
                    &woff_minor_version);
  plist::readObject(plist, "woffMetadataUniqueID",
                    &woff_metadata_unique_id);
  plist::readObject(plist, "woffMetadataVendor",
                    &woff_metadata_vendor);
  plist::readObject(plist, "woffMetadataCredits",
                    &woff_metadata_credits);
  plist::readObject(plist, "woffMetadataDescription",
                    &woff_metadata_description);
  plist::readObject(plist, "woffMetadataLicense",
                    &woff_metadata_license);
  plist::readObject(plist, "woffMetadataCopyright",
                    &woff_metadata_copyright);
  plist::readObject(plist, "woffMetadataTrademark",
                    &woff_metadata_trademark);
  plist::readObject(plist, "woffMetadataLicensee",
                    &woff_metadata_licensee);
  plist::readVector(plist, "woffMetadataExtensions",
                    &woff_metadata_extensions);
}

void FontInfo::readGuidelines(const PropertyList& plist) {
  plist::readVector(plist, "guidelines", &guidelines);
}

// MARK: Writing to property list

void FontInfo::writeIdentificationInformation(const PropertyList& plist) const {
  plist::writeString(plist, "familyName", family_name);
  plist::writeString(plist, "styleName", style_name);
  plist::writeString(plist, "styleMapFamilyName", style_map_family_name);
  plist::writeString(plist, "styleMapStyleName", style_map_style_name);
  plist::writeNumber(plist, "versionMajor", version_major);
  plist::writeNumber(plist, "versionMinor", version_minor);
  plist::writeNumber(plist, "year", year);
}

void FontInfo::writeLegalInformation(const PropertyList& plist) const {
  plist::writeString(plist, "copyright", copyright);
  plist::writeString(plist, "trademark", trademark);
}

void FontInfo::writeDimensionInformation(const PropertyList& plist) const {
  plist::writeNumber(plist, "unitsPerEm", units_per_em);
  plist::writeNumber(plist, "descender", descender);
  plist::writeNumber(plist, "xHeight", x_height);
  plist::writeNumber(plist, "capHeight", cap_height);
  plist::writeNumber(plist, "ascender", ascender);
  plist::writeNumber(plist, "italicAngle", italic_angle);
}

void FontInfo::writeMiscellaneousInformation(const PropertyList& plist) const {
  plist::writeString(plist, "note", note);
}

void FontInfo::writeOpenTypeGASPTableFields(const PropertyList& plist) const {
  plist::writeVector(plist, "openTypeGaspRangeRecords",
                     open_type_gasp_range_records);
}

void FontInfo::writeOpenTypeHEADTableFields(const PropertyList& plist) const {
  plist::writeString(plist, "openTypeHeadCreated",
                     open_type_head_created);
  plist::writeNumber(plist, "openTypeHeadLowestRecPPEM",
                     open_type_head_lowest_rec_ppem);
  plist::writeVector(plist, "openTypeHeadFlags",
                     open_type_head_flags);
}

void FontInfo::writeOpenTypeHHEATableFields(const PropertyList& plist) const {
  plist::writeNumber(plist, "openTypeHheaAscender",
                     open_type_hhea_ascender);
  plist::writeNumber(plist, "openTypeHheaDescender",
                     open_type_hhea_descender);
  plist::writeNumber(plist, "openTypeHheaLineGap",
                     open_type_hhea_line_gap);
  plist::writeNumber(plist, "openTypeHheaCaretSlopeRise",
                     open_type_hhea_caret_slope_rise);
  plist::writeNumber(plist, "openTypeHheaCaretSlopeRun",
                     open_type_hhea_caret_slope_run);
  plist::writeNumber(plist, "openTypeHheaCaretOffset",
                     open_type_hhea_caret_offset);
}

void FontInfo::writeOpenTypeNameTableFields(const PropertyList& plist) const {
  plist::writeString(plist, "openTypeNameDesigner",
                     open_type_name_designer);
  plist::writeString(plist, "openTypeNameDesignerURL",
                     open_type_name_designer_url);
  plist::writeString(plist, "openTypeNameManufacturer",
                     open_type_name_manufacturer);
  plist::writeString(plist, "openTypeNameManufacturerURL",
                     open_type_name_manufacturer_url);
  plist::writeString(plist, "openTypeNameLicense",
                     open_type_name_license);
  plist::writeString(plist, "openTypeNameLicenseURL",
                     open_type_name_license_url);
  plist::writeString(plist, "openTypeNameVersion",
                     open_type_name_version);
  plist::writeString(plist, "openTypeNameUniqueID",
                     open_type_name_unique_id);
  plist::writeString(plist, "openTypeNameDescription",
                     open_type_name_description);
  plist::writeString(plist, "openTypeNamePreferredFamilyName",
                     open_type_name_preferred_family_name);
  plist::writeString(plist, "openTypeNamePreferredSubfamilyName",
                     open_type_name_preferred_subfamily_name);
  plist::writeString(plist, "openTypeNameCompatibleFullName",
                     open_type_name_compatible_full_name);
  plist::writeString(plist, "openTypeNameSampleText",
                     open_type_name_sample_text);
  plist::writeString(plist, "openTypeNameWWSFamilyName",
                     open_type_name_wws_family_name);
  plist::writeString(plist, "openTypeNameWWSSubfamilyName",
                     open_type_name_wws_subfamily_name);
  plist::writeVector(plist, "openTypeNameRecords",
                     open_type_name_records);
}

void FontInfo::writeOpenTypeOS2TableFields(const PropertyList& plist) const {
  plist::writeNumber(plist, "openTypeOS2WidthClass",
                     open_type_os2_width_class);
  plist::writeNumber(plist, "openTypeOS2WeightClass",
                     open_type_os2_weight_class);
  plist::writeVector(plist, "openTypeOS2Selection",
                     open_type_os2_selection);
  plist::writeString(plist, "openTypeOS2VendorID",
                     open_type_os2_vendor_id);
  plist::writeVector(plist, "openTypeOS2Panose",
                     open_type_os2_panose);
  plist::writeVector(plist, "openTypeOS2FamilyClass",
                     open_type_os2_family_class);
  plist::writeVector(plist, "openTypeOS2UnicodeRanges",
                     open_type_os2_unicode_ranges);
  plist::writeVector(plist, "openTypeOS2CodePageRanges",
                     open_type_os2_code_page_ranges);
  plist::writeNumber(plist, "openTypeOS2TypoAscender",
                     open_type_os2_typo_ascender);
  plist::writeNumber(plist, "openTypeOS2TypoDescender",
                     open_type_os2_typo_descender);
  plist::writeNumber(plist, "openTypeOS2TypoLineGap",
                     open_type_os2_typo_line_gap);
  plist::writeNumber(plist, "openTypeOS2WinAscent",
                     open_type_os2_win_ascent);
  plist::writeNumber(plist, "openTypeOS2WinDescent",
                     open_type_os2_win_descent);
  plist::writeVector(plist, "openTypeOS2Type",
                     open_type_os2_type);
  plist::writeNumber(plist, "openTypeOS2SubscriptXSize",
                     open_type_os2_subscript_x_size);
  plist::writeNumber(plist, "openTypeOS2SubscriptYSize",
                     open_type_os2_subscript_y_size);
  plist::writeNumber(plist, "openTypeOS2SubscriptXOffset",
                     open_type_os2_subscript_x_offset);
  plist::writeNumber(plist, "openTypeOS2SubscriptYOffset",
                     open_type_os2_subscript_y_offset);
  plist::writeNumber(plist, "openTypeOS2SuperscriptXSize",
                     open_type_os2_superscript_x_size);
  plist::writeNumber(plist, "openTypeOS2SuperscriptYSize",
                     open_type_os2_superscript_y_size);
  plist::writeNumber(plist, "openTypeOS2SuperscriptXOffset",
                     open_type_os2_superscript_x_offset);
  plist::writeNumber(plist, "openTypeOS2SuperscriptYOffset",
                     open_type_os2_superscript_y_offset);
  plist::writeNumber(plist, "openTypeOS2StrikeoutSize",
                     open_type_os2_strikeout_size);
  plist::writeNumber(plist, "openTypeOS2StrikeoutPosition",
                     open_type_os2_strikeout_position);
}

void FontInfo::writeOpenTypeVHEATableFields(const PropertyList& plist) const {
  plist::writeNumber(plist, "openTypeVheaVertTypoAscender",
                     open_type_vhea_vert_typo_ascender);
  plist::writeNumber(plist, "openTypeVheaVertTypoDescender",
                     open_type_vhea_vert_typo_descender);
  plist::writeNumber(plist, "openTypeVheaVertTypoLineGap",
                     open_type_vhea_vert_typo_line_gap);
  plist::writeNumber(plist, "openTypeVheaCaretSlopeRise",
                     open_type_vhea_caret_slope_rise);
  plist::writeNumber(plist, "openTypeVheaCaretSlopeRun",
                     open_type_vhea_caret_slope_run);
  plist::writeNumber(plist, "openTypeVheaCaretOffset",
                     open_type_vhea_caret_offset);
}

void FontInfo::writePostScriptSpecificData(const PropertyList& plist) const {
  plist::writeString(plist, "postscriptFontName",
                     postscript_font_name);
  plist::writeString(plist, "postscriptFullName",
                     postscript_full_name);
  plist::writeNumber(plist, "postscriptSlantAngle",
                     postscript_slant_angle);
  plist::writeNumber(plist, "postscriptUniqueID",
                     postscript_unique_id);
  plist::writeNumber(plist, "postscriptUnderlineThickness",
                     postscript_underline_thickness);
  plist::writeNumber(plist, "postscriptUnderlinePosition",
                     postscript_underline_position);
  plist::writeBoolean(plist, "postscriptIsFixedPitch",
                      postscript_is_fixed_pitch);
  plist::writeVector(plist, "postscriptBlueValues",
                     postscript_blue_values);
  plist::writeVector(plist, "postscriptOtherBlues",
                     postscript_other_blues);
  plist::writeVector(plist, "postscriptFamilyBlues",
                     postscript_family_blues);
  plist::writeVector(plist, "postscriptFamilyOtherBlues",
                     postscript_family_other_blues);
  plist::writeVector(plist, "postscriptStemSnapH",
                     postscript_stem_snap_h);
  plist::writeVector(plist, "postscriptStemSnapV",
                     postscript_stem_snap_v);
  plist::writeNumber(plist, "postscriptBlueFuzz",
                     postscript_blue_fuzz);
  plist::writeNumber(plist, "postscriptBlueShift",
                     postscript_blue_shift);
  plist::writeNumber(plist, "postscriptBlueScale",
                     postscript_blue_scale);
  plist::writeBoolean(plist, "postscriptForceBold",
                      postscript_force_bold);
  plist::writeNumber(plist, "postscriptDefaultWidthX",
                     postscript_default_width_x);
  plist::writeNumber(plist, "postscriptNominalWidthX",
                     postscript_nominal_width_x);
  plist::writeString(plist, "postscriptWeightName",
                     postscript_weight_name);
  plist::writeString(plist, "postscriptDefaultCharacter",
                     postscript_default_character);
  plist::writeNumber(plist, "postscriptWindowsCharacterSet",
                     postscript_windows_character_set);
}

void FontInfo::writeMacintoshFONDResourceData(const PropertyList& plist) const {
  plist::writeNumber(plist, "macintoshFONDFamilyID",
                     macintosh_fond_family_id);
  plist::writeString(plist, "macintoshFONDName",
                     macintosh_fond_name);
}

void FontInfo::writeWOFFData(const PropertyList& plist) const {
  plist::writeNumber(plist, "woffMajorVersion",
                     woff_major_version);
  plist::writeNumber(plist, "woffMinorVersion",
                     woff_minor_version);
  plist::writeObject(plist, "woffMetadataUniqueID",
                     woff_metadata_unique_id);
  plist::writeObject(plist, "woffMetadataVendor",
                     woff_metadata_vendor);
  plist::writeObject(plist, "woffMetadataCredits",
                     woff_metadata_credits);
  plist::writeObject(plist, "woffMetadataDescription",
                     woff_metadata_description);
  plist::writeObject(plist, "woffMetadataLicense",
                     woff_metadata_license);
  plist::writeObject(plist, "woffMetadataCopyright",
                     woff_metadata_copyright);
  plist::writeObject(plist, "woffMetadataTrademark",
                     woff_metadata_trademark);
  plist::writeObject(plist, "woffMetadataLicensee",
                     woff_metadata_licensee);
  plist::writeVector(plist, "woffMetadataExtensions",
                     woff_metadata_extensions);
}

void FontInfo::writeGuidelines(const PropertyList& plist) const {
  plist::writeVector(plist, "guidelines", guidelines);
}

}  // namespace ufo
}  // namespace token
