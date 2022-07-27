#include "serializer.h"

#include "meta/meta_base_classes.h"
#include "meta/meta_info_spec.h"
#include "serialization/serializer_base.h"
#include "utils.h"

namespace reflection {

status object_serializer::serialize_to_string(const meta_object& object, std::string* output,
                                              format_context* context) {
  *output += "{";
  if (context) {
    *output += '\n';
    ++(context->tab_size);
    append_tab_space(*context, output);
  }
  bool has_push_item = false;
  for (auto it = object.get_meta_info().item_vec().begin();
       it != object.get_meta_info().item_vec().end(); ++it) {
    const meta_item* meta_item_ptr = *it;
    if (meta_item_ptr->get_serializer().skip(object, *meta_item_ptr)) {
      continue;
    }

    if (has_push_item) {
      *output += ",";
      if (context) {
        *output += '\n';
        append_tab_space(*context, output);
      }
    }

    append_name(meta_item_ptr->name(), output);
    if (context) {
      *output += ' ';
    }
    return_if_error(meta_item_ptr->get_serializer().serialize_to_string(object, *meta_item_ptr,
                                                                        output, context));
    has_push_item = true;
  }
  if (context) {
    *output += '\n';
    --(context->tab_size);
    append_tab_space(*context, output);
  }
  *output += "}";
  return status::ok();
}

status object_serializer::serialize_from_string(const std::string_view& str_value,
                                                meta_object* object) {
  std::string_view tmp_str_value = str_value;
  std::string_view tmp_result;
  return_if_error(expected_token("{", get_token(&tmp_str_value)));
  while (!tmp_str_value.empty()) {
    // Get item name or the `}`
    const std::string_view token = get_token(&tmp_str_value);
    if (token == "}") {
      return check_none_extra_token(tmp_str_value);
    } else if (!is_world(token)) {
      return status::error("Invalid item name format " + view_to_string(token));
    }
    const std::string item_name_str = view_to_string(token);
    if (!object->has_meta(item_name_str)) {
      return status::error("Class " + object->class_name() + " do not has member " + item_name_str);
    }

    const meta_item& item = object->get_meta_item(item_name_str);
    return_if_error(expected_token(":", get_token(&tmp_str_value)));

    std::string_view variable_part;
    return_if_error(get_variable_part(&tmp_str_value, &variable_part));

    item.get_serializer().serialize_from_string(variable_part, object, item);
    // Skip `,` or meetin the `}`
    const std::string_view post_token = get_token(&tmp_str_value);
    if (post_token == "}") {
      return check_none_extra_token(tmp_str_value);
    } else {
      return_if_error(expected_token(",", post_token));
    }
  }
  return status::error("Not match right parantheses for : " + view_to_string(str_value));
}

status object_serializer::serialize_to_bytes(const meta_object& object, endian endian_v,
                                             std::vector<char>* out) {
  for (const auto& item : object.get_meta_info().item_vec()) {
    if (item->get_serializer().skip(object, *item)) {
      continue;
    }
    {
      // Record name
      block_size_writer<uint16_t> _size_write(out, endian_v);
      binary_coder<std::string>::code(item->name(), endian_v, out);
    }
    {
      block_size_writer<uint32_t> _size_write(out, endian_v);
      return_if_error(item->get_serializer().serialize_to_bytes(object, *item, out, endian_v));
    }
  }
  return status::ok();
}

status object_serializer::serialize_from_bytes(const char* first, const char* last, endian endian_v,
                                               meta_object* object) {
  while (first < last) {
    std::string item_name;
    uint16_t name_size = 0;
    return_if_error(general_serializer<uint16_t>::serialize_from_bytes(
        first, first + sizeof(uint16_t), endian_v, &name_size));
    first += sizeof(uint16_t);
    item_name.reserve(name_size);
    return_if_error(general_serializer<std::string>::serialize_from_bytes(first, first + name_size,
                                                                          endian_v, &item_name));
    if (!object->has_meta(item_name)) {
      return status::error("object " + object->class_name() + " does not have " + item_name);
    }

    first += name_size;
    uint32_t item_size = 0;
    return_if_error(general_serializer<uint32_t>::serialize_from_bytes(
        first, first + sizeof(uint32_t), endian_v, &item_size));
    first += sizeof(uint32_t);
    const meta_item& item = object->get_meta_item(item_name);
    return_if_error(item.get_serializer().serialize_from_bytes(first, first + item_size, object,
                                                               item, endian_v));
    first += item_size;
  }
  return first == last ? status::ok() : status::error("Bytes size not match!");
}

} // namespace reflection