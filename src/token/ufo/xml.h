// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

#pragma once
#ifndef TOKEN_UFO_XML_H_
#define TOKEN_UFO_XML_H_

#include <cassert>
#include <memory>
#include <string>
#include <vector>

#include <boost/property_tree/ptree.hpp>

#include "token/ufo/optional.h"

namespace token {
namespace ufo {
namespace xml {

template <class T>
inline void readAttribute(const boost::property_tree::ptree& tree,
                          const std::string& name,
                          T *output) {
  assert(output);
  const auto attrs = tree.find("<xmlattr>");
  if (attrs != tree.not_found()) {
    const auto itr = attrs->second.find(name);
    if (itr != attrs->second.not_found()) {
      *output = itr->second.get_value<T>();
    }
  }
}

template <class T>
inline void readChild(const boost::property_tree::ptree& tree,
                      const std::string& name,
                      T *output) {
  assert(output);
  const auto itr = tree.find(name);
  if (itr != tree.not_found()) {
    *output = T(itr->second);
  }
}

template <class T>
inline void readChild(const boost::property_tree::ptree& tree,
                      const std::string& name,
                      Optional<T> *output) {
  assert(output);
  const auto itr = tree.find(name);
  if (itr != tree.not_found()) {
    *output = T(itr->second);
  }
}

template <class T>
inline void readChildren(const boost::property_tree::ptree& tree,
                         const std::string& name,
                         std::vector<T> *output) {
  assert(output);
  const auto values = tree.find(name);
  if (values != tree.not_found()) {
    auto end = values;
    std::advance(end, tree.count(name));
    for (auto itr = values; itr != end; ++itr) {
      output->emplace_back(itr->second);
    }
  }
}

template <class T>
inline void writeAttribute(boost::property_tree::ptree *tree,
                           const std::string& name,
                           const T& value) {
  assert(tree);
  tree->put("<xmlattr>." + name, value);
}

template <class T, class U>
inline void writeAttribute(boost::property_tree::ptree *tree,
                           const std::string& name,
                           const T& value,
                           const U& default_value) {
  assert(tree);
  if (value != default_value) {
    tree->put("<xmlattr>." + name, value);
  }
}

template <class T>
inline void writeChild(boost::property_tree::ptree *tree,
                       const std::string& name,
                       const T& value) {
  assert(tree);
  tree->add_child(name, value.ptree());
}

template <class T>
inline void writeChild(boost::property_tree::ptree *tree,
                       const std::string& name,
                       const Optional<T>& value) {
  assert(tree);
  if (value.exists()) {
    tree->add_child(name, value->ptree());
  }
}

template <class T>
inline void writeChildren(boost::property_tree::ptree *tree,
                          const std::string& name,
                          const std::vector<T>& values) {
  assert(tree);
  for (const auto& value : values) {
    tree->add_child(name, value.ptree());
  }
}

}  // namespace xml
}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_XML_H_
