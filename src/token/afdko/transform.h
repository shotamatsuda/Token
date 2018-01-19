// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#pragma once
#ifndef TOKEN_AFDKO_TRANSFORM_H_
#define TOKEN_AFDKO_TRANSFORM_H_

#include <string>

#include <boost/property_tree/ptree.hpp>

namespace token {
namespace afdko {

bool convertFontToXML(const std::string& directory,
                      const std::string& input,
                      boost::property_tree::ptree *tree);
bool convertXMLToFont(const std::string& directory,
                      const std::string& output,
                      const boost::property_tree::ptree& tree);

template <class Transformer>
inline bool transformFont(const std::string& directory,
                          const std::string& input,
                          Transformer transformer) {
  boost::property_tree::ptree tree;
  if (!convertFontToXML(directory, input, &tree)) {
    return false;
  }
  transformer(tree);
  return convertXMLToFont(directory, input, tree);
}

}  // namespace afdko
}  // namespace token

#endif  // TOKEN_AFDKO_TRANSFORM_H_
