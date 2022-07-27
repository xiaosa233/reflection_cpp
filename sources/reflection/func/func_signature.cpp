#include "func/func_signature.h"

#include <regex>
#include <sstream>

#include "serialization/utils.h"

namespace reflection {

template <class Limiter, class Iter>
std::string join(const Limiter& limiter, Iter first, Iter last) {
  std::stringstream ss;
  while (first != last) {
    ss << first->to_string();
    if (++first != last) {
      ss << limiter;
    }
  }
  return ss.str();
}

bool func_args::to(const func_args& other) const {
  if (type_size != other.type_size)
    return false;
  if (type == other.type)
    return true;
  if (decay_type == other.decay_type) {
    if (!is_const && !(is_const_pointer && !other.is_const_pointer)) {
      // There will be a const pointer, but it is not const attributes.
      return true;
    }
    if ((other.is_const || (!other.is_lvalue_reference && !other.is_rvalue_reference)) &&
        !(is_const_pointer && !other.is_const_pointer))
      return true;
  }
  return false;
}

std::string func_args::to_string() const {
  return type + "_" + std::to_string(type_size);
}

bool func_signature::to(const func_signature& other) const {
  if (!(return_type.to(other.return_type) && args_type.size() == other.args_type.size())) {
    return false;
  }
  for (const auto& arg : args_type) {
    if (!arg.to(other.args_type[&arg - args_type.data()])) {
      return false;
    }
  }
  return true;
}

std::string func_signature::to_string() const {
  return return_type.to_string() + "(" + join(',', args_type.begin(), args_type.end()) + ")";
}

namespace func_signature_detail {

std::string get_decay_type(const std::string_view& all_type) {
  //(const )?([a-zA-z0-9_]+\**)&*
  static const std::regex base_regex("(const )?([a-zA-z0-9_]+\\**)(const)?&*");
  std::match_results<std::string_view::const_iterator> result_match;
  if (!std::regex_match(all_type.begin(), all_type.end(), result_match, base_regex)) {
    return view_to_string(all_type);
  }
  return result_match.size() > 2 ? view_to_string(result_match[2].str()) : view_to_string(all_type);
}

} // namespace func_signature_detail

} // namespace reflection
