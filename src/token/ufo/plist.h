//
//  token/ufo/plist.h
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
#ifndef TOKEN_UFO_PLIST_H_
#define TOKEN_UFO_PLIST_H_

extern "C" {

#include <plist/plist.h>

}  // extern "C"

#include <cassert>
#include <cstdint>
#include <cstdlib>
#include <functional>
#include <string>
#include <vector>

#include "token/ufo/optional.h"

namespace token {
namespace ufo {
namespace plist {

template <class T>
inline void read_number(plist_t plist, const std::string& name, T *output) {
  assert(output);
  assert(plist_get_node_type(plist) == PLIST_DICT);
  const auto node = plist_dict_get_item(plist, name.c_str());
  if (node) {
    const auto type = plist_get_node_type(node);
    if (type == PLIST_BOOLEAN) {
      std::uint8_t value{};
      plist_get_bool_val(node, &value);
      *output = value;
    } else if (type == PLIST_UINT) {
      std::uint64_t value{};
      plist_get_uint_val(node, &value);
      *output = *reinterpret_cast<std::int64_t *>(&value);
    } else if (type == PLIST_REAL) {
      double value{};
      plist_get_real_val(node, &value);
      *output = value;
    }
  }
}

template <class T>
inline void read_string(plist_t plist, const std::string& name, T *output) {
  assert(output);
  assert(plist_get_node_type(plist) == PLIST_DICT);
  const auto node = plist_dict_get_item(plist, name.c_str());
  if (node) {
    assert(plist_get_node_type(node) == PLIST_STRING);
    char *value{};
    plist_get_string_val(node, &value);
    *output = value;
    std::free(value);
  }
}

template <class T>
inline void read_object(plist_t plist, const std::string& name, T *output) {
  assert(output);
  assert(plist_get_node_type(plist) == PLIST_DICT);
  const auto node = plist_dict_get_item(plist, name.c_str());
  if (node) {
    assert(plist_get_node_type(node) == PLIST_DICT);
    *output = T::read(PropertyList(node, false));
  }
}

template <class T>
inline void read_object(plist_t plist,
                        const std::string& name,
                        Optional<T> *output) {
  assert(output);
  assert(plist_get_node_type(plist) == PLIST_DICT);
  const auto node = plist_dict_get_item(plist, name.c_str());
  if (node) {
    assert(plist_get_node_type(node) == PLIST_DICT);
    *output = T::read(PropertyList(node, false));
  }
}

template <class T, std::enable_if_t<std::is_class<T>::value> * = nullptr>
inline void read_vector(plist_t plist,
                        const std::string& name,
                        std::vector<T> *output) {
  assert(output);
  assert(plist_get_node_type(plist) == PLIST_DICT);
  const auto node = plist_dict_get_item(plist, name.c_str());
  if (node) {
    assert(plist_get_node_type(node) == PLIST_DICT);
    output->emplace_back(T::read(PropertyList(node, false)));
  }
}

template <class T, std::enable_if_t<std::is_arithmetic<T>::value> * = nullptr>
inline void read_vector(plist_t plist,
                        const std::string& name,
                        std::vector<T> *output) {
  assert(output);
  assert(plist_get_node_type(plist) == PLIST_DICT);
  const auto node = plist_dict_get_item(plist, name.c_str());
  if (node) {
    assert(plist_get_node_type(node) == PLIST_ARRAY);
    const auto size = plist_array_get_size(node);
    for (std::uint32_t i{}; i < size; ++i) {
      const auto item = plist_array_get_item(node, i);
      const auto type = plist_get_node_type(item);
      if (type == PLIST_BOOLEAN) {
        std::uint8_t value{};
        plist_get_bool_val(item, &value);
        output->emplace_back(value);
      } else if (type == PLIST_UINT) {
        std::uint64_t value{};
        plist_get_uint_val(item, &value);
        output->emplace_back(*reinterpret_cast<std::int64_t *>(&value));
      } else if (type == PLIST_REAL) {
        double value{};
        plist_get_real_val(item, &value);
        output->emplace_back(value);
      }
    }
  }
}

}  // namespace plist
}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_PLIST_H_
