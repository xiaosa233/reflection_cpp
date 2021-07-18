
#pragma once

#include <array>
#include <limits>
#include <sstream>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

#include "serialization\utils.h"
#include "status.h"

namespace reflection {

// Only work for numbers
template <class T>
struct text_code_arithmetic {
  static_assert(std::is_arithmetic_v<T> && !std::is_same_v<T, bool> && !std::is_same_v<T, char>,
                "Should not arrive here. Maybe you need to define a specialization-template "
                "struct text_coder");
  static status code(T inst, std::string* output) {
    std::ostringstream ss;
    if (std::numeric_limits<T>::max_digits10) {
      ss << std::setprecision(std::numeric_limits<T>::max_digits10);
    }
    ss << inst;
    if (ss.rdstate() == std::ios::goodbit) {
      *output += ss.str();
      return status::ok();
    }
    return status::error("Failed in the data to string!");
  }

  static status decode(const std::string_view& str_value, T* inst) {
    std::istringstream ss(view_to_string(str_value));
    ss >> *inst;
    if (!ss.fail()) {
      return status::ok();
    }
    return status::error(std::string("Failed in the string to data!") + view_to_string(str_value));
  }
};

template <class T>
struct text_coder {
  static status code(const T& inst, std::string* output) {
    static_assert(std::is_arithmetic_v<T> && !std::is_same_v<T, bool> && !std::is_same_v<T, char>,
                  "Should not arrive here. Maybe you need to define a specialization-template "
                  "struct text_coder");
    return text_code_arithmetic<T>::code(inst, output);
  }

  static status decode(const std::string_view& str_value, T* inst) {
    return text_code_arithmetic<T>::decode(str_value, inst);
  }
};

template <>
struct text_coder<bool> {
  static status code(bool inst, std::string* output) {
    *output += inst ? "true" : "false";
    return status::ok();
  }

  static status decode(const std::string_view& str_value, bool* inst) {
    if (str_value == "true") {
      *inst = true;
    } else if (str_value == "false") {
      *inst = false;
    } else {
      return status::error("bool can only equal to true or false");
    }
    return status::ok();
  }
};

template <>
struct text_coder<char> {
  static status code(char inst, std::string* output) {
    *output += inst;
    return status::ok();
  };

  static status decode(const std::string_view& str_value, char* inst) {
    if (!str_value.empty()) {
      *inst = str_value.front();
    }
    return str_value.size() == 1 ? status::ok()
                                 : status::error("Size of the string to cahar is not equal to " +
                                                 std::to_string(str_value.size()));
  }
};

template <>
struct text_coder<std::string> {
  static status code(const std::string& inst, std::string* output) {
    *output += '\"';
    *output += inst;
    *output += '\"';
    return status::ok();
  }

  static status decode(const std::string_view& str_value, std::string* inst) {
    if (str_value.size() > 1 && str_value.front() == '\"' && str_value.back() == '\"') {
      *inst = str_value.substr(1, str_value.size() - 2);
      return status::ok();
    }
    return status::error("string value without \"" + view_to_string(str_value));
  }
};

} // namespace reflection