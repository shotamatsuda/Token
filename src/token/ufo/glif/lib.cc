// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#include "token/ufo/glif/lib.h"

#include <string>
#include <sstream>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "token/ufo/glif/contour_styles.h"
#include "token/ufo/plist.h"
#include "token/ufo/property_list.h"

namespace token {
namespace ufo {
namespace glif {

// MARK: Property tree

Lib::Lib(const boost::property_tree::ptree& tree) : Lib() {
  auto plist = convertToPropertyList(tree);
  plist::readNumber(plist, "com.shotamatsuda.token.numberOfContours",
                    &number_of_contours);
  plist::readNumber(plist, "com.shotamatsuda.token.numberOfHoles",
                    &number_of_holes);
  plist::readObject(plist, "com.shotamatsuda.token.contourStyles",
                    &contour_styles);
}

boost::property_tree::ptree Lib::ptree() const {
  boost::property_tree::ptree dict;
  dict.add("key", "com.shotamatsuda.token.numberOfContours");
  dict.add("integer", number_of_contours);
  dict.add("key", "com.shotamatsuda.token.numberOfHoles");
  dict.add("integer", number_of_holes);
  if (!contour_styles.empty()) {
    dict.add("key", "com.shotamatsuda.token.contourStyles");
    dict.add_child("dict", contour_styles.ptree());
  }
  boost::property_tree::ptree tree;
  tree.add_child("dict", dict);
  return tree;
}

PropertyList Lib::convertToPropertyList(
    const boost::property_tree::ptree& tree) {
  std::ostringstream stream;
  boost::property_tree::xml_parser::write_xml(stream, tree);
  auto contents = stream.str();

  // Erase the xml declaration
  contents.erase(0, contents.find("\n") + 1);

  // WORKAROUND: Erase all of the xml escape sequences
  const std::string line_feed = "&#10;";
  const auto line_feed_size = line_feed.size();
  for (auto i = contents.find(line_feed_size);
       i != std::string::npos;
       i = contents.find(line_feed_size)) {
    contents.erase(i, line_feed_size);
  }
  const std::string plist_contents =
      "<?xml version=\"1.0\" encoding=\"UTF-8\"?>"
      "<!DOCTYPE plist PUBLIC \"-//Apple Computer//DTD PLIST 1.0//EN\" "
      "\"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">"
      "<plist version=\"1.0\">" + contents + "</plist>";
  plist_t node{};
  plist_from_xml(plist_contents.c_str(), plist_contents.size(), &node);
  assert(plist_get_node_type(node) == PLIST_DICT);
  return PropertyList(node);
}

}  // namespace glif
}  // namespace ufo
}  // namespace token
