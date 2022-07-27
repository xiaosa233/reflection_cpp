#include "serialization/utils.h"

#include <regex>
#include <sstream>
#include <string>
#include <unordered_set>

namespace reflection {

namespace {
const std::unordered_set<char> special_char = {'[', ']', ',', ':', '{', '}'};

const char* match_right_terminator(const char* begin, const char* end, char left_terminator,
                                   char right_terminator) {
  int match_left_terminator = 1;
  for (const char* curr = begin; curr < end; ++curr) {
    if (*curr == '\"') {
      // skip content wrap by the ""
      for (++curr; curr < end && *curr != '\"'; ++curr) {
        if (*curr == '\\') {
          ++curr;
        }
      }
      if (*curr != '\"') {
        return curr;
      }
      continue;
    }
    if (*curr == left_terminator) {
      ++match_left_terminator;
    } else if (*curr == right_terminator && --match_left_terminator == 0) {
      return curr;
    }
  }
  return end;
}

} // namespace

void append_name(const std::string& name, std::string* output) {
  if (!output)
    return;
  if (name.size()) {
    *output += name + ":";
  }
}

void append_tab_space(const format_context& context, std::string* output) {
  for (int i = 0; i < context.tab_size * context.tab_space_count; ++i) {
    *output += " ";
  }
}

status skip_name(const std::string_view& name, std::string_view* str_view) {
  if (!name.empty()) {
    std::string_view token_name;
    return_if_error(get_word(str_view, &token_name));
    return_if_error(expected_token(name, token_name));
    return_if_error(expected_token(":", get_token(str_view)));
  }
  return status::ok();
}

bool is_world(const std::string_view& str_view) {
  return str_view.size() > 1 || (str_view.size() == 1 && !special_char.count(str_view.front()));
}

std::string_view get_token(std::string_view* str_view) {
  std::match_results<std::string_view::const_iterator> match_result;
  // [^ \n\0:,\{\}\[\]]+|:|\{|\}|\:|,|\[|\]
  // \"((\\.)|.)*?\"  --> for string match
  //(\"((\\.)|.)*?\")|([^ \n\0:,\{\}\[\]]+)|:|\{|\}|\:|,|\[|\]
  static const std::regex token_regex(
      "(\\\"((\\\\.)|.)*?\\\")|([^ \\n\\0:,\\{\\}\\[\\]]+)|:|\\{|\\}|\\:|,|\\[|\\]");
  if (std::regex_search(str_view->begin(), str_view->end(), match_result, token_regex)) {
    const char* new_begin_data = str_view->data() + match_result.position() + match_result.length();
    *str_view =
        std::string_view(new_begin_data, str_view->data() + str_view->size() - new_begin_data);
    return std::string_view(new_begin_data - match_result.length(), match_result.length());
  }
  return {};
}

status get_word(std::string_view* str_view, std::string_view* out_world) {
  const std::string_view token = get_token(str_view);
  if (is_world(token)) {
    *out_world = token;
    return status::ok();
  }
  return status::error(view_to_string(token) + " is not a world!");
}

status get_variable_part(std::string_view* str_view, std::string_view* out_variable) {
  std::string_view backup_str_view = *str_view;
  std::string_view token = get_token(&backup_str_view);
  if (token == "{" || token == "[") {
    const char* match_pos = nullptr;
    const char* end_pos = backup_str_view.data() + backup_str_view.size();
    if (token == "{") {
      match_pos = match_right_terminator(backup_str_view.data(), end_pos, '{', '}');
    } else {
      match_pos = match_right_terminator(backup_str_view.data(), end_pos, '[', ']');
    }
    if (match_pos != end_pos) {
      *out_variable = std::string_view(token.data(), match_pos + 1 - token.data());
      *str_view = std::string_view(match_pos + 1, end_pos - 1 - match_pos);
      return status::ok();
    }
    return status::error(std::string("Can not match ") + (token == "{" ? "}" : "]") +
                         " for : " + view_to_string(*str_view));
  } else if (is_world(token)) {
    *out_variable = token;
    *str_view = backup_str_view;
    return status::ok();
  }
  return status::error("Can not get a valid general part from : " + view_to_string(*str_view));
}

status expected_token(const std::string_view expected_token, const std::string_view& token) {
  if (expected_token != token) {
    return status::error("Should equal to " + view_to_string(expected_token) + " vs " +
                         view_to_string(token));
  }
  return status::ok();
}

bool startswith(const std::string_view& str_value, const std::string_view& starts_str) {
  return str_value.size() >= starts_str.size() &&
         str_value.substr(0, starts_str.size()) == starts_str;
}

bool endswith(const std::string_view& str_view, const std::string_view& pattern) {
  if (str_view.size() >= pattern.size()) {
    return str_view.substr(str_view.size() - 1 - pattern.size()) == pattern;
  }
  return false;
}

status check_none_extra_token(const std::string_view& str_view) {
  std::string_view tmp = str_view;
  return get_token(&tmp).empty() ? status::ok()
                                 : status::error("Extra string: " + view_to_string(str_view));
}

} // namespace reflection