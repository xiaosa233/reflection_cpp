#pragma once

#include <string_view>
#include <vector>

#include "serialization/binary_coder.h"
#include "serialization/format_context.h"
#include "serialization/text_coder.h"
#include "serialization/trait_utils.h"
#include "serialization/utils.h"

namespace reflection {

// TODO Add serializer of the map and set.

/*
The serializer corresponds to the serialization of different types.
The interfaces required by each serializer are as follows:

  static status serialize_to_string(const T& inst, std::string* output,
                                    format_context* context);

  static status serialize_from_string(const std::string_view& str_value, T* inst);

  static status serialize_to_bytes(const T& inst, endian endian_v,
                                   std::vector<char>* out);
  static status serialize_from_bytes(const char* first, const char* last, endian endian_v,
                                     T* inst);
And optinoal function as follow:
  static bool skip(const T& value);

*/

class meta_object;

// write the binary size meta
template <class T>
struct block_size_writer;

/* object serialzier output a scope of the meta object instance. E.g.
person: {
    name: jack
    id: 234
}
* */
struct object_serializer {
  static status serialize_to_string(const meta_object& object, std::string* output,
                                    format_context* context);

  static status serialize_from_string(const std::string_view& str_value, meta_object* object);

  static status serialize_to_bytes(const meta_object& object, endian endian_v,
                                   std::vector<char>* out);
  static status serialize_from_bytes(const char* first, const char* last, endian endian_v,
                                     meta_object* object);
};

// general serializer does not output and end characters.
// E.g. id: 234
template <class T>
struct general_serializer {
  static status serialize_to_string(const T& value, std::string* output,
                                    format_context* /*context*/) {
    return text_coder<T>::code(value, output);
  }

  static status serialize_from_string(const std::string_view& str_value, T* value) {
    std::string_view tmp_string_view = str_value;
    std::string_view variable_part;
    return_if_error(get_word(&tmp_string_view, &variable_part));
    return_if_error(text_coder<T>::decode(variable_part, value));
    return check_none_extra_token(tmp_string_view);
  }

  static status serialize_to_bytes(const T& inst, endian endian_v, std::vector<char>* out) {
    return binary_coder<T>::code(inst, endian_v, out);
  }

  static status serialize_from_bytes(const char* first, const char* last, endian endian_v,
                                     T* inst) {
    return binary_coder<T>::decode(first, last, endian_v, inst);
  }
};

template <class T>
struct optional_serializer {
  // Should not arrive here!
};

template <class T>
struct vector_serializer {
  // Should not array here.
};

template <class T, class... Args>
struct vector_serializer<std::vector<T, Args...>> {
  using vector_type = std::vector<T, Args...>;

  using serializer_type =
      typename selector<reflected_type<T>::value, object_serializer, vector_serializer<T>,
                        optional_serializer<T>, general_serializer<T>>::type;

  static bool skip(const vector_type& value) { return value.empty(); }

  static status serialize_to_string(const vector_type& value, std::string* output,
                                    format_context* context) {
    *output += "[";
    size_t new_line_size = std::numeric_limits<size_t>::max();
    if (context) {
      new_line_size = std::is_trivial_v<typename vector_type::value_type>
                          ? context->trivial_value_new_line_size
                          : 1;
    }
    if (value.size() > new_line_size) {
      *output += '\n';
      ++(context->tab_size);
      append_tab_space(*context, output);
    }
    bool has_push_item = false;
    for (auto it = value.begin(); it != value.end(); ++it) {
      if (skip_help(*it, static_cast<serializer_type*>(nullptr))) {
        continue;
      }
      if (has_push_item) {
        std::ptrdiff_t index = it - value.begin();
        if (context) {
          if (index != 0 && (index % new_line_size) == 0) {
            *output += ",\n";
            append_tab_space(*context, output);
          } else {
            *output += ", ";
          }
        } else {
          *output += ",";
        }
      }
      return_if_error(serializer_type::serialize_to_string(*it, output, context));
      has_push_item = true;
    }
    if (value.size() > new_line_size) {
      *output += "\n";
      --(context->tab_size);
      append_tab_space(*context, output);
    }
    *output += "]";
    return status::ok();
  }

  static status serialize_from_string(const std::string_view& str_value, vector_type* value) {
    std::string_view tmp_str_view = str_value;
    return_if_error(expected_token("[", get_token(&tmp_str_view)));

    while (!tmp_str_view.empty()) {
      // maybe variable part or the ']'
      std::string_view may_be_variable_part;
      if (get_variable_part(&tmp_str_view, &may_be_variable_part)) {
        value->emplace_back();
        return_if_error(
            serializer_type::serialize_from_string(may_be_variable_part, &value->back()));
        // skip `,` or meet the ']'
        const std::string_view post_token = get_token(&tmp_str_view);
        if (post_token == "]") {
          return check_none_extra_token(tmp_str_view);
        } else {
          return_if_error(expected_token(",", post_token));
        }
      } else {
        return_if_error(expected_token("]", get_token(&tmp_str_view)));
        return check_none_extra_token(tmp_str_view);
      }
    }
    return status::error("Can not match right brackets for : " + view_to_string(str_value));
  }

  static status serialize_to_bytes(const vector_type& inst, endian endian_v,
                                   std::vector<char>* out) {
    return serialize_to_bytes_help<std::is_trivial_v<T>>(inst, endian_v, out);
  }

  static status serialize_from_bytes(const char* first, const char* last, endian endian_v,
                                     vector_type* inst) {
    return serialize_from_bytes_help<std::is_trivial_v<T>>(first, last, endian_v, inst);
  }

private:
  template <bool is_trivial>
  static std::enable_if_t<is_trivial, status> serialize_to_bytes_help(const vector_type& inst, endian endian_v,
                                        std::vector<char>* out) {
    return binary_coder<vector_type>::code(inst, endian_v, out);
  }

  template <bool is_trivial>
  static std::enable_if_t<!is_trivial, status> serialize_to_bytes_help(const vector_type& inst, endian endian_v,
                                                std::vector<char>* out) {
    for (const auto& it : inst) {
      if (skip_help(it, static_cast<serializer_type*>(nullptr))) {
        continue;
      }
      block_size_writer<uint32_t> _size_writer(out, endian_v);
      return_if_error(serializer_type::serialize_to_bytes(it, endian_v, out));
    }
    return status::ok();
  }

  template <bool is_trivial>
  static std::enable_if_t<is_trivial, status> serialize_from_bytes_help(const char* first, const char* last, endian endian_v,
                                          vector_type* inst) {
    return binary_coder<vector_type>::decode(first, last, endian_v, inst);
  }

  template <bool is_trivial>
  static std::enable_if_t<!is_trivial, status> serialize_from_bytes_help(const char* first, const char* last,
                                                  endian endian_v, vector_type* inst) {
    while (first < last) {
      uint32_t block_size = 0;
      return_if_error(general_serializer<uint32_t>::serialize_from_bytes(
          first, first + sizeof(uint32_t), endian_v, &block_size));
      first += sizeof(uint32_t);
      inst->emplace_back();
      return_if_error(serializer_type::serialize_from_bytes(first, first + block_size, endian_v,
                                                            &inst->back()));
      first += block_size;
    }
    return first == last ? status::ok() : status::error("Not match bytes size");
  }

  template <class SerializerT>
  static std::enable_if_t<std::is_function_v<decltype(SerializerT::skip)>, bool>
  skip_help(const T& value, SerializerT*) {
    return SerializerT::skip(value);
  }

  static bool skip_help(const T& value, void*) { return false; }
};

template <class T>
struct block_size_writer {

  static_assert(std::is_integral_v<T>, "T must be a integral type!");
  static constexpr size_t type_size = sizeof(T);

  block_size_writer(std::vector<char>* buffer_in, endian endian_in)
      : buffer(buffer_in), endian_v(endian_in) {
    prev_size = buffer->size();
    buffer->resize(buffer->size() + type_size);
  }
  ~block_size_writer() {
    T* size_ptr = reinterpret_cast<T*>(buffer->data() + prev_size);
    *size_ptr = static_cast<T>(buffer->size() - prev_size - type_size);
    if (endian_v != endian::native) {
      byte_swapper<type_size>::swap(size_ptr);
    }
  }

  std::vector<char>* buffer;
  size_t prev_size = 0;
  endian endian_v;
};

template <class T>
struct optional_serializer<std::optional<T>> {

    // replace code here.
  using serializer_type =
      typename selector<reflected_type<T>::value, object_serializer, vector_serializer<T>,
                        optional_serializer<T>, general_serializer<T>>::type;

  static bool skip(const std::optional<T>& inst) { return !inst; }
  static status serialize_to_string(const std::optional<T>& inst, std::string* output,
                                    format_context* context) {
    if (inst) {
      return serializer_type::serialize_to_string(*inst, output, context);
    }
    return status::ok();
  }

  static status serialize_from_string(const std::string_view& str_value, std::optional<T>* inst) {
    std::string_view tmp_str_view = str_value;
    std::string_view variable_part;
    return_if_error(get_variable_part(&tmp_str_view, &variable_part));
    if (!variable_part.empty()) {
      inst->emplace();
      return serializer_type::serialize_from_string(variable_part, &inst->value());
    }
    return check_none_extra_token(tmp_str_view);
  }

  static status serialize_to_bytes(const std::optional<T>& inst, endian endian_v,
                                   std::vector<char>* out) {
    if (inst) {
      return serializer_type::serialize_to_bytes(*inst, endian_v, out);
    }
    return status::ok();
  }
  static status serialize_from_bytes(const char* first, const char* last, endian endian_v,
                                     std::optional<T>* inst) {
    if (first <= last) {
      inst->emplace();
      return serializer_type::serialize_from_bytes(first, last, endian_v, &inst->value());
    }
    return status::ok();
  }
};

template<class T>
struct serializer_type {
  using type =
    typename selector<reflected_type<T>::value, object_serializer, vector_serializer<T>,
                      optional_serializer<T>, general_serializer<T>>::type;
  };

} // namespace reflection
