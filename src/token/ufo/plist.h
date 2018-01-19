// The MIT License
// Copyright (C) 2015-Present Shota Matsuda

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
#include "token/ufo/property_list.h"

namespace token {
namespace ufo {
namespace plist {

inline void readBoolean(plist_t plist, const std::string& name, bool *output) {
  assert(output);
  assert(plist_get_node_type(plist) == PLIST_DICT);
  const auto node = plist_dict_get_item(plist, name.c_str());
  if (node) {
    assert(plist_get_node_type(node) == PLIST_BOOLEAN);
    std::uint8_t value{};
    plist_get_bool_val(node, &value);
    *output = value;
  }
}

inline void readBoolean(plist_t plist,
                        const std::string& name,
                        Optional<bool> *output) {
  assert(output);
  assert(plist_get_node_type(plist) == PLIST_DICT);
  const auto node = plist_dict_get_item(plist, name.c_str());
  if (node) {
    assert(plist_get_node_type(node) == PLIST_BOOLEAN);
    std::uint8_t value{};
    plist_get_bool_val(node, &value);
    *output = value;
  }
}

template <class T>
inline void readNumber(plist_t plist, const std::string& name, T *output) {
  assert(output);
  assert(plist_get_node_type(plist) == PLIST_DICT);
  const auto node = plist_dict_get_item(plist, name.c_str());
  if (node) {
    const auto type = plist_get_node_type(node);
    if (type == PLIST_UINT) {
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
inline void readString(plist_t plist, const std::string& name, T *output) {
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
inline void readObject(plist_t plist, const std::string& name, T *output) {
  assert(output);
  assert(plist_get_node_type(plist) == PLIST_DICT);
  const auto node = plist_dict_get_item(plist, name.c_str());
  if (node) {
    assert(plist_get_node_type(node) == PLIST_DICT);
    *output = T(PropertyList(node, false));
  }
}

template <class T>
inline void readObject(plist_t plist,
                       const std::string& name,
                       Optional<T> *output) {
  assert(output);
  assert(plist_get_node_type(plist) == PLIST_DICT);
  const auto node = plist_dict_get_item(plist, name.c_str());
  if (node) {
    assert(plist_get_node_type(node) == PLIST_DICT);
    *output = T(PropertyList(node, false));
  }
}

template <
  class T,
  std::enable_if_t<std::is_arithmetic<T>::value> * = nullptr
>
inline void readVector(plist_t plist,
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
      if (type == PLIST_UINT) {
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

template <
  class T,
  std::enable_if_t<std::is_class<T>::value> * = nullptr
>
inline void readVector(plist_t plist,
                       const std::string& name,
                       std::vector<T> *output) {
  assert(output);
  assert(plist_get_node_type(plist) == PLIST_DICT);
  const auto node = plist_dict_get_item(plist, name.c_str());
  if (node) {
    assert(plist_get_node_type(node) == PLIST_DICT);
    output->emplace_back(T(PropertyList(node, false)));
  }
}

inline void writeBoolean(plist_t plist, const std::string& name, bool value) {
  assert(plist_get_node_type(plist) == PLIST_DICT);
  const auto node = plist_new_bool(value);
  plist_dict_set_item(plist, name.c_str(), node);
}

inline void writeBoolean(plist_t plist,
                         const std::string& name,
                         const Optional<bool>& value) {
  assert(plist_get_node_type(plist) == PLIST_DICT);
  if (value.exists()) {
    const auto node = plist_new_bool(value);
    plist_dict_set_item(plist, name.c_str(), node);
  }
}

template <
  class T,
  std::enable_if_t<std::is_integral<T>::value> * = nullptr
>
inline void writeNumber(plist_t plist, const std::string& name, T value) {
  assert(plist_get_node_type(plist) == PLIST_DICT);
  const auto node = plist_new_uint(value);
  plist_dict_set_item(plist, name.c_str(), node);
}

template <
  class T,
  std::enable_if_t<std::is_floating_point<T>::value> * = nullptr
>
inline void writeNumber(plist_t plist, const std::string& name, T value) {
  assert(plist_get_node_type(plist) == PLIST_DICT);
  const auto node = plist_new_real(value);
  plist_dict_set_item(plist, name.c_str(), node);
}

template <
  class T,
  std::enable_if_t<std::is_integral<T>::value> * = nullptr
>
inline void writeNumber(plist_t plist,
                        const std::string& name,
                        const Optional<T>& value) {
  assert(plist_get_node_type(plist) == PLIST_DICT);
  if (value.exists()) {
    const auto node = plist_new_uint(value);
    plist_dict_set_item(plist, name.c_str(), node);
  }
}

template <
  class T,
  std::enable_if_t<std::is_floating_point<T>::value> * = nullptr
>
inline void writeNumber(plist_t plist,
                        const std::string& name,
                        const Optional<T>& value) {
  assert(plist_get_node_type(plist) == PLIST_DICT);
  if (value.exists()) {
    const auto node = plist_new_real(value);
    plist_dict_set_item(plist, name.c_str(), node);
  }
}

inline void writeString(plist_t plist,
                        const std::string& name,
                        const std::string& value) {
  assert(plist_get_node_type(plist) == PLIST_DICT);
  if (value.empty()) {
    return;
  }
  const auto node = plist_new_string(value.c_str());
  plist_dict_set_item(plist, name.c_str(), node);
}

template <
  class T,
  std::enable_if_t<std::is_object<T>::value> * = nullptr
>
inline void writeObject(plist_t plist,
                        const std::string& name,
                        const T& value) {
  assert(plist_get_node_type(plist) == PLIST_DICT);
  plist_dict_set_item(plist, name.c_str(), value.plist().release());
}

template <
  class T,
  std::enable_if_t<std::is_object<T>::value> * = nullptr
>
inline void writeObject(plist_t plist,
                        const std::string& name,
                        const Optional<T>& value) {
  assert(plist_get_node_type(plist) == PLIST_DICT);
  if (value.exists()) {
    plist_dict_set_item(plist, name.c_str(), value->plist().release());
  }
}

template <
  class T,
  std::enable_if_t<std::is_integral<T>::value> * = nullptr
>
inline void writeVector(plist_t plist,
                        const std::string& name,
                        const std::vector<T>& values) {
  assert(plist_get_node_type(plist) == PLIST_DICT);
  if (values.empty()) {
    return;
  }
  const auto node = plist_new_array();
  for (const auto& value : values) {
    const auto item = plist_new_uint(value);
    plist_array_append_item(node, item);
  }
  plist_dict_set_item(plist, name.c_str(), node);
}

template <
  class T,
  std::enable_if_t<std::is_floating_point<T>::value> * = nullptr
>
inline void writeVector(plist_t plist,
                        const std::string& name,
                        const std::vector<T>& values) {
  if (values.empty()) {
    return;
  }
  assert(plist_get_node_type(plist) == PLIST_DICT);
  const auto node = plist_new_array();
  for (const auto& value : values) {
    const auto item = plist_new_real(value);
    plist_array_append_item(node, item);
  }
  plist_dict_set_item(plist, name.c_str(), node);
}

template <
  class T,
  std::enable_if_t<std::is_class<T>::value> * = nullptr
>
inline void writeVector(plist_t plist,
                        const std::string& name,
                        const std::vector<T>& values) {
  if (values.empty()) {
    return;
  }
  assert(plist_get_node_type(plist) == PLIST_DICT);
  const auto node = plist_new_array();
  for (const auto& value : values) {
    plist_array_append_item(node, value.plist().release());
  }
  plist_dict_set_item(plist, name.c_str(), node);
}

}  // namespace plist
}  // namespace ufo
}  // namespace token

#endif  // TOKEN_UFO_PLIST_H_
