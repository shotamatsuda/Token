// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#pragma once
#ifndef TOKEN_UFO_GLYPH_H_
#define TOKEN_UFO_GLYPH_H_

#include <istream>
#include <ostream>
#include <string>
#include <vector>

#include <boost/property_tree/ptree.hpp>

#include "token/ufo/glif.h"

namespace token {
namespace ufo {

class Glyph final {
 public:
  Glyph() = default;
  explicit Glyph(const std::string& path);

  // Copy semantics
  Glyph(const Glyph&) = default;
  Glyph& operator=(const Glyph&) = default;

  // Opening and saving
  bool open(const std::string& path);
  bool open(std::istream& stream);
  bool save(const std::string& path) const;
  bool save(std::ostream& stream) const;

 public:
  std::string name;
  Optional<glif::Advance> advance;
  std::vector<glif::Unicode> unicodes;
  Optional<glif::Image> image;
  std::vector<glif::Guideline> guidelines;
  std::vector<glif::Anchor> anchors;
  Optional<glif::Outline> outline;
  Optional<glif::Lib> lib;
};

// Comparison
bool operator==(const Glyph& lhs, const Glyph& rhs);
bool operator!=(const Glyph& lhs, const Glyph& rhs);

// MARK: -

inline Glyph::Glyph(const std::string& path) {
  open(path);
}

// MARK: Comparison

inline bool operator==(const Glyph& lhs, const Glyph& rhs) {
  return (lhs.name == rhs.name &&
          lhs.advance == rhs.advance &&
          lhs.unicodes == rhs.unicodes &&
          lhs.image == rhs.image &&
          lhs.guidelines == rhs.guidelines &&
          lhs.anchors == rhs.anchors &&
          lhs.outline == rhs.outline &&
          lhs.lib == rhs.lib);
}

inline bool operator!=(const Glyph& lhs, const Glyph& rhs) {
  return !(lhs == rhs);
}

}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_GLYPH_H_
