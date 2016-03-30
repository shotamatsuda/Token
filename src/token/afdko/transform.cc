//
//  token/afdko/transform.cc
//
//  The MIT License
//
//  Copyright (C) 2015-2016 Shota Matsuda
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

#include "token/afdko/transform.h"

#include <cassert>
#include <fstream>
#include <sstream>
#include <string>

#include <boost/algorithm/string/replace.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "token/afdko/task.h"

namespace token {
namespace afdko {

bool convertFontToXML(const std::string& directory,
                      const std::string& input,
                      boost::property_tree::ptree *tree) {
  assert(tree);
  Task task;
  task.set_directory(directory);
  task.set_name("ttx");
  task.set_arguments({"-f", input});
  if (!task.execute()) {
    return false;
  }
  const boost::filesystem::path path(input);
  const auto xml_path = path.parent_path() / (path.stem().string() + "ttx");
  boost::property_tree::xml_parser::read_xml(xml_path.string(), *tree);
  return true;
}

bool convertXMLToFont(const std::string& directory,
                      const std::string& output,
                      const boost::property_tree::ptree& tree) {
  std::ostringstream oss;
  boost::property_tree::xml_writer_settings<std::string> settings(' ', 2);
  boost::property_tree::xml_parser::write_xml(oss, tree, settings);

  // Manually replace line feeds then save to file because boost's xml parser
  // doesn't encode them.
  const boost::filesystem::path path(output);
  const auto xml_path = path.parent_path() / (path.stem().string() + "ttx");
  std::string xml = oss.str();
  boost::replace_all(xml, "&#10;", "\x0a");
  std::ofstream file(xml_path.string(), std::ios::binary | std::ios::trunc);
  file << xml;
  file.close();

  // Convert ttx back to the font format
  Task task;
  task.set_directory(directory);
  task.set_name("ttx");
  task.set_arguments({"-f", xml_path.string()});
  return task.execute();
}

}  // namespace afdko
}  // namespace token
