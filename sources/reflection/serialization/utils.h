#pragma once

#include <string>
#include <vector>

#include "serialization/format_context.h"
#include "status.h"

namespace reflection {

void append_name(const std::string& name, std::string* output);

void append_tab_space(const format_context& context, std::string* output);

status skip_name(const std::string_view& name, std::string_view* str_view);

// get token from the string view
std::string_view get_token(std::string_view* str_view);

status expected_token(const std::string_view expected_token, const std::string_view& token);

// get a world without the single token. E.g. ',' '{' '}, etc.
status get_word(std::string_view* str_view, std::string_view* out_world);

// get variable part. For object {/../}, vector [/.../]. others a world.
status get_variable_part(std::string_view* str_view, std::string_view* out_variable);

bool startswith(const std::string_view& str_value, const std::string_view& starts_str);

bool endswith(const std::string_view& str_view, const std::string_view& pattern);

bool is_world(const std::string_view& str_view);

status check_none_extra_token(const std::string_view& str_view);

inline std::string view_to_string(const std::string_view& str_view) {
  return std::string(str_view.data(), str_view.size());
}

} // namespace reflection