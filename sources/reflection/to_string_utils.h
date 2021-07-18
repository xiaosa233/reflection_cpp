#pragma once

#include <string>
#include <string_view>

#include "serialization/serializer.h"
#include "serialization/utils.h"

namespace reflection {

namespace utils {

template <class T>
std::string to_string(const T& value) {
  std::string result;
  serializer_type<T>::type::serialize_to_string(value, &result, nullptr);
  return result;
}

template <>
inline std::string to_string<std::string>(const std::string& value) {
  return value;
}

template <class T>
T from_string(const std::string_view& str_view) {
  T result;
  serializer_type<T>::type::serialize_from_string(str_view, &result);
  return result;
}

template <>
inline std::string from_string<std::string>(const std::string_view& str_view) {
  return view_to_string(str_view);
}

} // namespace utils
} // namespace reflection
