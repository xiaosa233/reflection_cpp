#include "meta/meta_base_classes.h"

#include "meta/meta_manager.h"
#include "serialization/serializer_base.h"
#include "serialization/utils.h"

namespace reflection {
const meta_info& meta_object::get_meta_info() const {
  static meta_info inst("default_meta_info");
  return inst;
}
const func_info& meta_object::get_func_info() const {
  static func_info inst("default_func_info");
  return inst;
}
const serializer_base& meta_object::get_serializer() const {
  static serializer_base inst;
  return inst;
}

const func_item& meta_object::get_func_item(const std::string_view& name) const {
  return get_func_info().get_func_item(view_to_string(name));
}

status meta_object::serialize_to_string(std::string* out, format_context* context) const {
  return get_serializer().serialize_to_string(*this, out, context);
}

status meta_object::serialize_from_string(const std::string_view& str_view) {
  return get_serializer().serialize_from_string(str_view, this);
}

status meta_object::member_to_string(const std::string& name, std::string* out) const {
  return_if_error(check_has(name));
  const meta_item& item = get_meta_item(name);
  return item.get_serializer().serialize_to_string(*this, item, out, nullptr);
}

status meta_object::member_from_string(const std::string& name, const std::string_view& str_view) {
  return_if_error(check_has(name));
  const meta_item& item = get_meta_item(name);
  return item.get_serializer().serialize_from_string(str_view, this, item);
}

status meta_object::serialize_to_bytes(std::vector<char>* out, endian endian_v) const {
  return get_serializer().serialize_to_bytes(*this, out, endian_v);
}

status meta_object::member_to_bytes(const std::string_view& name, std::vector<char>* out,
                                    endian endian_v) const {
  return_if_error(check_has(name));
  const meta_item& item = get_meta_item(view_to_string(name));
  return item.get_serializer().serialize_to_bytes(*this, out, endian_v);
}

status meta_object::serialize_from_bytes(const char* first, const char* last, endian endian_v) {
  return get_serializer().serialize_from_bytes(first, last, this, endian_v);
}

status meta_object::member_from_bytes(const std::string_view& name, const char* first,
                                      const char* last, endian endian_v) {
  return_if_error(check_has(name));
  const meta_item& item = get_meta_item(view_to_string(name));
  return item.get_serializer().serialize_from_bytes(first, last, this, endian_v);
}

status meta_object::check_has(const std::string_view& name) const {
  return has_meta(name)
             ? status::ok()
             : status::error("class " + class_name() + " does not have " + view_to_string(name));
}

meta_info::meta_info(const char* class_name) : class_name_(class_name) {
  meta_info_manager::inst().push(class_name, *this);
};

} // namespace reflection
