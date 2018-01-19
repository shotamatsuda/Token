// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

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
  const auto xml_path = path.parent_path() / (path.stem().string() + ".ttx");
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
  const auto xml_path = path.parent_path() / (path.stem().string() + ".ttx");
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
