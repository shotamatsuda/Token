// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#pragma once
#ifndef TOKEN_UFO_GUIDELINE_H_
#define TOKEN_UFO_GUIDELINE_H_

#include <string>

#include "token/ufo/property_list.h"

namespace token {
namespace ufo {

class Guideline final {
 public:
  Guideline() = default;
  Guideline(double x,
            double y,
            double angle,
            const std::string& name,
            const std::string& color,
            const std::string& identifier);

  // Copy semantics
  Guideline(const Guideline&) = default;
  Guideline& operator=(const Guideline&) = default;

  // Property list
  explicit Guideline(const PropertyList& plist);
  PropertyList plist() const;

 public:
  double x;
  double y;
  double angle;
  std::string name;
  std::string color;
  std::string identifier;
};

// Comparison
bool operator==(const Guideline& lhs, const Guideline& rhs);
bool operator!=(const Guideline& lhs, const Guideline& rhs);

// MARK: -

inline Guideline::Guideline(double x,
                            double y,
                            double angle,
                            const std::string& name,
                            const std::string& color,
                            const std::string& identifier)
    : x(x),
      y(y),
      angle(angle),
      name(name),
      color(color),
      identifier(identifier) {}

// MARK: Comparison

inline bool operator==(const Guideline& lhs, const Guideline& rhs) {
  return (lhs.x == rhs.x &&
          lhs.y == rhs.y &&
          lhs.angle == rhs.angle &&
          lhs.name == rhs.name &&
          lhs.color == rhs.color &&
          lhs.identifier == rhs.identifier);
}

inline bool operator!=(const Guideline& lhs, const Guideline& rhs) {
  return !(lhs == rhs);
}

}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_GUIDELINE_H_
