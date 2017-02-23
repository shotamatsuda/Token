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

#include "token/ufo/glif/lib.h"

#include <string>
#include <sstream>
#include <utility>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "token/ufo/plist.h"
#include "token/ufo/property_list.h"

namespace token {
namespace ufo {
namespace glif {

// MARK: Property tree

Lib::Lib(const boost::property_tree::ptree& tree) : Lib() {
  auto plist = convertToPropertyList(tree);
  plist::read_number(plist, "com.takram.token.numberOfContours",
                     &number_of_contours);
  plist::read_number(plist, "com.takram.token.numberOfHoles",
                     &number_of_holes);
}

boost::property_tree::ptree Lib::ptree() const {
  boost::property_tree::ptree dict;
  dict.add("key", "com.takram.token.numberOfContours");
  dict.add("integer", number_of_contours);
  dict.add("key", "com.takram.token.numberOfHoles");
  dict.add("integer", number_of_holes);
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
