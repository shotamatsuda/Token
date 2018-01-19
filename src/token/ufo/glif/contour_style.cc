// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#include "token/ufo/glif/contour_style.h"

#include <string>

#include <boost/property_tree/ptree.hpp>

#include "token/types.h"
#include "token/ufo/plist.h"
#include "token/ufo/property_list.h"

namespace token {
namespace ufo {
namespace glif {

namespace {

inline Cap convertCap(const std::string& value) {
  if (value == "butt") {
    return Cap::BUTT;
  } else if (value == "round") {
    return Cap::ROUND;
  } else if (value == "project") {
    return Cap::PROJECT;
  }
  return Cap::UNDEFINED;
}

inline std::string convertCap(Cap value) {
  switch (value) {
    case Cap::BUTT:
      return "butt";
    case Cap::ROUND:
      return "round";
    case Cap::PROJECT:
      return "project";
    default:
      break;
  }
  return "";
}

inline Join convertJoin(const std::string& value) {
  if (value == "miter") {
    return Join::MITER;
  } else if (value == "round") {
    return Join::ROUND;
  } else if (value == "bevel") {
    return Join::BEVEL;
  }
  return Join::UNDEFINED;
}

inline std::string convertJoin(Join value) {
  switch (value) {
    case Join::MITER:
      return "miter";
    case Join::ROUND:
      return "round";
    case Join::BEVEL:
      return "bevel";
    default:
      break;
  }
  return "";
}

inline Align convertAlign(const std::string& value) {
  if (value == "none") {
    return Align::NONE;
  } else if (value == "left") {
    return Align::LEFT;
  } else if (value == "right") {
    return Align::RIGHT;
  }
  return Align::UNDEFINED;
}

inline std::string convertAlign(Align value) {
  switch (value) {
    case Align::NONE:
      return "none";
    case Align::LEFT:
      return "left";
    case Align::RIGHT:
      return "right";
    default:
      break;
  }
  return "";
}

}  // namespace

// MARK: Property list

ContourStyle::ContourStyle(const PropertyList& plist) {
  std::string value;
  plist::readString(plist, "cap", &value);
  cap = convertCap(value);
  value = std::string();
  plist::readString(plist, "join", &value);
  join = convertJoin(value);
  value = std::string();
  plist::readString(plist, "align", &value);
  align = convertAlign(value);
  plist::readBoolean(plist, "filled", &filled);
}

PropertyList ContourStyle::plist() const {
  PropertyList plist;
  plist::writeString(plist, "cap", convertCap(cap));
  plist::writeString(plist, "join", convertJoin(join));
  plist::writeString(plist, "align", convertAlign(align));
  if (filled) {
    plist::writeBoolean(plist, "filled", true);
  }
  return plist;
}

boost::property_tree::ptree ContourStyle::ptree() const {
  boost::property_tree::ptree tree;
  if (cap != Cap::UNDEFINED) {
    tree.add("key", "cap");
    tree.add("string", convertCap(cap));
  }
  if (join != Join::UNDEFINED) {
    tree.add("key", "join");
    tree.add("string", convertJoin(join));
  }
  if (align != Align::UNDEFINED) {
    tree.add("key", "align");
    tree.add("string", convertAlign(align));
  }
  if (filled) {
    tree.add("key", "filled");
    tree.add("true", "");
  }
  return tree;
}

}  // namespace glif
}  // namespace ufo
}  // namespace token
