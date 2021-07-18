#pragma once

#include <string>
#include <vector>

namespace reflection {

class meta_object;

struct func_args {
  bool is_const = false;
  bool is_lvalue_reference = false;
  bool is_rvalue_reference = false;
  bool is_const_pointer = false;
  // bool is_pointer = false;
  std::string decay_type;
  std::string type;
  size_t type_size = 0;

  // Allow none-const --> const
  bool to(const func_args& other) const;
  std::string to_string() const;
};

struct func_signature {
  func_args return_type;
  std::vector<func_args> args_type;

  bool to(const func_signature& other) const;

  std::string to_string() const;
};

template <class... Args>
struct trait_signature_helper {
  static func_signature trait() {
    static_assert(false, "Should not arrive!");
    return {};
  }
};

// ------- Implementation --------

namespace func_signature_detail {

template <class T>
std::string type_name(...) {
  return "unknown";
}

template <class T,
          std::enable_if_t<!std::is_const_v<T> && std::is_integral_v<T> && std::is_signed_v<T>,
                           bool> = true>
std::string type_name() {
  return "signed_integral";
}

template <class T,
          std::enable_if_t<!std::is_const_v<T> && std::is_integral_v<T> && std::is_unsigned_v<T>,
                           bool> = true>
std::string type_name() {
  return "unsigned_integral";
}

template <class T,
          std::enable_if_t<!std::is_const_v<T> && std::is_floating_point_v<T>, bool> = true>
std::string type_name() {
  return "floating_point";
}

template <class T, std::enable_if_t<std::is_convertible_v<T*, meta_object*>, bool> = true>
std::string type_name() {
  return "meta_object_sub_class";
}

template <class T, std::enable_if_t<std::is_same_v<T, std::string>, bool> = true>
std::string type_name() {
  return "string";
}

// const but not pointer
template <class T, std::enable_if_t<std::is_const_v<T> && !std::is_pointer_v<T>, bool> = true>
std::string type_name() {
  return "const " + type_name<std::remove_const_t<T>>();
}

template <class T, std::enable_if_t<std::is_lvalue_reference_v<T>, bool> = true>
std::string type_name() {
  return type_name<std::remove_reference_t<T>>() + "&";
}

template <class T, std::enable_if_t<std::is_rvalue_reference_v<T>, bool> = true>
std::string type_name() {
  return type_name<std::remove_reference_t<T>>() + "&&";
}

// pointer but not const, note const int* is not a const pointer.
template <class T, std::enable_if_t<std::is_pointer_v<T> && !std::is_const_v<T>, bool> = true>
std::string type_name() {
  return type_name<std::remove_pointer_t<T>>() + "*";
}

// int* const , const int* const
template <class T, std::enable_if_t<std::is_pointer_v<T> && std::is_const_v<T>, bool> = true>
std::string type_name() {
  return type_name<std::remove_const_t<T>>() + "const";
}

// TODO: add typename for vector/map/queue, etc. containers.

std::string get_decay_type(const std::string_view& all_type);

template <class T>
inline func_args trait_args() {
  func_args result;
  result.type = type_name<T>();
  result.decay_type = get_decay_type(result.type);
  result.is_const = std::is_const_v<std::remove_reference_t<T>>;
  result.is_lvalue_reference = std::is_lvalue_reference_v<T>;
  result.is_rvalue_reference = std::is_rvalue_reference_v<T>;
  result.is_const_pointer = std::is_pointer_v<std::remove_reference_t<T>> &&
                            std::is_const_v<std::remove_pointer_t<std::remove_reference_t<T>>>;
  result.type_size = sizeof(T);
  return result;
}

template <>
inline func_args trait_args<void>() {
  func_args result;
  result.type = "void";
  result.decay_type = "void";
  result.type_size = 0;
  return result;
}

} // namespace func_signature_detail

template <class R, class... Args>
struct trait_signature_helper<R(Args...)> {
  static func_signature trait() {
    func_signature result;
    result.return_type = func_signature_detail::trait_args<R>();
    result.args_type = {func_signature_detail::trait_args<Args>()...};
    return result;
  }
};

} // namespace reflection
