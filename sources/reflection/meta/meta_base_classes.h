#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>

#include "common.h"
#include "func/func_base_classes.h"
#include "serialization/endian.h"
#include "serialization/format_context.h"
#include "serialization/utils.h"
#include "status.h"

namespace reflection {

class meta_item;
class func_item;
class meta_object;
class serializer_base;

template <class T>
class meta_item_mem_type;

class meta_info {
public:
  meta_info(const char* class_name);
  virtual ~meta_info() = default;

  std::string class_name() const { return class_name_; }

  virtual std::unique_ptr<meta_object> create() const { return {}; }

  inline const std::unordered_map<std::string, const meta_item*>& item_map() const {
    return items_;
  }

  // item vector. Keep the order with the declared-order.
  inline const std::vector<const meta_item*>& item_vec() const { return item_vec_; }

  bool has(const std::string_view& name) const { return items_.count(view_to_string(name)); }

  const meta_item& get_meta_item(const std::string& name) const { return *items_.at(name); }

protected:
  const char* class_name_;
  std::unordered_map<std::string, const meta_item*> items_;
  std::vector<const meta_item*> item_vec_;

  DISALLOW_COPY_AND_MOVE(meta_info);
};

class meta_item {
public:
  meta_item(const char* type, const char* name) : type_(type), name_(name){};
  virtual ~meta_item() = default;

  virtual const serializer_base& get_serializer() const = 0;

  inline const char* name() const { return name_; }
  inline const char* type() const { return type_; }

  template <class T>
  const T& get(const meta_object* ptr) const {
    return static_cast<const meta_item_mem_type<T>*>(this)->get(ptr);
  }
  template <class T>
  T* mutable_get(meta_object* ptr) const {
    return static_cast<const meta_item_mem_type<T>*>(this)->mutable_get(ptr);
  }

  template <class T>
  void set(meta_object* ptr, const T& value) const {
    // convert const char* or char[] to std::string
    using type = std::conditional_t<std::is_pointer_v<std::decay_t<T>> &&
                                        std::is_convertible_v<T, std::string>,
                                    std::string, T>;
    return static_cast<const meta_item_mem_type<type>*>(this)->set(ptr, value);
  }
  template <class T>
  void set(meta_object* ptr, T&& value) const {
    using type = std::conditional_t<std::is_pointer_v<std::decay_t<T>> &&
                                        std::is_convertible_v<T, std::string>,
                                    std::string, T>;
    return static_cast<const meta_item_mem_type<type>*>(this)->set(ptr, std::move(value));
  }

private:
  const char* type_;
  const char* name_;

  DISALLOW_COPY_AND_MOVE(meta_item);
};

class meta_object {
public:
  virtual ~meta_object() = default;
  virtual const meta_info& get_meta_info() const;
  virtual const func_info& get_func_info() const;

  virtual const serializer_base& get_serializer() const;

  std::string class_name() const { return get_meta_info().class_name(); }

  bool has_meta(const std::string_view& meta_name) const { return get_meta_info().has(meta_name); }

  bool has_func(const std::string_view& func_name) const { return get_func_info().has(func_name); }

  const meta_item& get_meta_item(const std::string& name) const {
    return get_meta_info().get_meta_item(name);
  }

  const func_item& get_func_item(const std::string_view& name) const;

  template <class return_type = void, class... Args>
  context_status<return_type> invoke(const std::string_view& func_name, Args&&... args) {
    if (!has_func(func_name)) {
      return context_status<return_type>::error(class_name() + " do not has func " +
                                                view_to_string(func_name));
    }
    const func_item& item = get_func_item(func_name);
    if (!item.is_static()) {
      return item.invoke<return_type>(this, std::forward<Args>(args)...);
    } else {
      return item.invoke<return_type>(std::forward<Args>(args)...);
    }
  }

  template <class return_type = void, class... Args>
  context_status<return_type> invoke(const std::string_view& func_name, Args&&... args) const {
    return const_cast<meta_object*>(this)->invoke<return_type>(func_name,
                                                               std::forward<Args>(args)...);
  }

  string_status invoke_by_string(const std::string_view& func_name,
                                 const std::vector<std::string>& args) {
    if (!has_func(func_name)) {
      return string_status::error(class_name() + " do not has func " + view_to_string(func_name));
    }
    const func_item& item = get_func_item(func_name);
    meta_object* inst_ptr = item.is_static() ? nullptr : this;
    return item.invoke_by_string(args, inst_ptr);
  }
  string_status invoke_by_string(const std::string_view& func_name,
                                 const std::vector<std::string>& args) const {
    return const_cast<meta_object*>(this)->invoke_by_string(func_name, args);
  }

  template <class T>
  const T& get(const std::string& name) const {
    return get_meta_info().get_meta_item(name).get<T>(this);
  }
  template <class T>
  T* mutable_get(const std::string_view& name) const {
    return get_meta_info().get_meta_item(view_to_string(name)).mutable_get<T>(this);
  }

  template <class T>
  meta_object& set(const std::string& name, const T& value) {
    get_meta_info().get_meta_item(name).set<T>(this, value);
    return *this;
  }
  template <class T>
  meta_object& set(std::string& name, T&& value) {
    get_meta_info().get_meta_item(name).set<T>(this, std::move(value));
    return *this;
  }

  status serialize_to_string(std::string* out, format_context* context = nullptr) const;

  status serialize_from_string(const std::string_view& str_view);

  status member_to_string(const std::string& name, std::string* out) const;

  status member_from_string(const std::string& name, const std::string_view& str_view);

  // TODO: remove interface to the serialize with bytes.
  status serialize_to_bytes(std::vector<char>* out, endian endian_v = endian::native) const;

  status member_to_bytes(const std::string_view& name, std::vector<char>* out,
                         endian endian_v = endian::native) const;

  status serialize_from_bytes(const char* first, const char* last,
                              endian endian_v = endian::native);

  status member_from_bytes(const std::string_view& name, const char* first, const char* last,
                           endian endian_v = endian::big);

protected:
  status check_has(const std::string_view& name) const;
};

} // namespace reflection
