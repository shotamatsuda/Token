//
//  token/ufo/xml.h
//
//  The MIT License
//
//  Copyright (C) 2015 Shota Matsuda
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
inline void read_attr(const boost::property_tree::ptree& tree,
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
inline void read_child(const boost::property_tree::ptree& tree,
                       const std::string& name,
                       T *output) {
  assert(output);
  const auto itr = tree.find(name);
  if (itr != tree.not_found()) {
    *output = T::read(itr->second);
  }
}

template <class T>
inline void read_child(const boost::property_tree::ptree& tree,
                       const std::string& name,
                       Optional<T> *output) {
  assert(output);
  const auto itr = tree.find(name);
  if (itr != tree.not_found()) {
    *output = T::read(itr->second);
  }
}

template <class T>
inline void read_children(const boost::property_tree::ptree& tree,
                          const std::string& name,
                          std::vector<T> *output) {
  assert(output);
  const auto values = tree.find(name);
  if (values != tree.not_found()) {
    auto end = values;
    std::advance(end, tree.count(name));
    for (auto itr = values; itr != end; ++itr) {
      output->emplace_back(T::read(itr->second));
    }
  }
}

template <class T>
inline void write_attr(boost::property_tree::ptree *tree,
                       const std::string& name,
                       const T& value) {
  assert(tree);
  tree->put("<xmlattr>." + name, value);
}

template <class T, class U>
inline void write_attr(boost::property_tree::ptree *tree,
                       const std::string& name,
                       const T& value,
                       const U& default_value) {
  assert(tree);
  if (value != default_value) {
    tree->put("<xmlattr>." + name, value);
  }
}

template <class T>
inline void write_child(boost::property_tree::ptree *tree,
                        const std::string& name,
                        const T& value) {
  assert(tree);
  tree->add_child(name, value.ptree());
}

template <class T>
inline void write_child(boost::property_tree::ptree *tree,
                        const std::string& name,
                        const Optional<T>& value) {
  assert(tree);
  if (value.exists()) {
    tree->add_child(name, value->ptree());
  }
}

template <class T>
inline void write_children(boost::property_tree::ptree *tree,
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
