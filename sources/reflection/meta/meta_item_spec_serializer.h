#pragma once

#include <string_view>
#include <type_traits>

#include "common.h"
#include "meta/meta_base_classes.h"
#include "serialization/serializer.h"
#include "serialization/serializer_base.h"
#include "serialization/trait_utils.h"

namespace reflection {

template <class T, class MemT>
struct member_pointer {
  using type = MemT(T::*);
};

template <class T, class MemT>
using member_pointer_t = typename member_pointer<T, MemT>::type;

template <class T>
class meta_item_mem_type : public meta_item {
public:
  // TODO: fixed, can not accept pointer and can convert to std::string
  // static_assert(!std::is_pointer_v<std::decay_t<T>>, "Not support reflection on pointer");
  meta_item_mem_type(const char* type, const char* name) : meta_item(type, name) {}
  ~meta_item_mem_type() override = default;
  virtual const T& get(const meta_object* ptr) const = 0;
  virtual T* mutable_get(meta_object* ptr) const = 0;
  virtual void set(meta_object* ptr, const T& value) const = 0;
  virtual void set(meta_object* ptr, T&& value) const = 0;
  const serializer_base& get_serializer() const override {
    static serializer<T> serializer_v;
    return serializer_v;
  }
};

template <class T, class MemT, member_pointer_t<T, MemT> Ptr>
class meta_item_spec : public meta_item_mem_type<MemT> {
  static_assert(std::is_convertible_v<T*, meta_object*>, " T is not a meta_object derived class!");

public:
  meta_item_spec(const char* type, const char* name) : meta_item_mem_type<MemT>(type, name) {}
  ~meta_item_spec() override = default;

  const MemT& get(const meta_object* ptr) const override {
    return static_cast<const T*>(ptr)->*Ptr;
  }

  MemT* mutable_get(meta_object* ptr) const override { return &(static_cast<T*>(ptr)->*Ptr); }

  void set(meta_object* ptr, const MemT& value) const override {
    static_cast<T*>(ptr)->*Ptr = value;
  }
  void set(meta_object* ptr, MemT&& value) const override {
    static_cast<T*>(ptr)->*Ptr = std::move(value);
  }
};

template <class T, bool is_meta_oject = true>
struct meta_item_self_wrapper : public meta_item_mem_type<T> {
  static_assert(std::is_convertible_v<T*, meta_object*>, " T is not a meta_object derived class!");

public:
  static constexpr const char* self_class_type = "self_class_type";
  meta_item_self_wrapper() : meta_item_mem_type<T>(self_class_type, "") {}
  ~meta_item_self_wrapper() override = default;

  const T& get(const meta_object* ptr) const override { return *static_cast<const T*>(ptr); }
  T* mutable_get(meta_object* ptr) const override { return static_cast<T*>(ptr); }
  void set(meta_object* ptr, const T& value) const override { *static_cast<T*>(ptr) = value; }
  void set(meta_object* ptr, T&& value) const override { *static_cast<T*>(ptr) = std::move(value); }
};

template <class T>
struct meta_item_self_wrapper<T, false> : public meta_item {
  meta_item_self_wrapper() : meta_item(nullptr, nullptr) {
    // Never Should not run to here!, just make compiler happy
  }

  const serializer_base& get_serializer() const {
    static serializer_base _mock_inst;
    return _mock_inst;
  }
};

/*
 * The serializer and meta_item_spec are put together because they have a have loop-back
 * dependencies.
 */
template <class T>
struct serializer : public serializer_base {
  using serializer_type = typename serializer_type<T>::type;

  bool skip(const meta_object& object, const meta_item& item) const override {
    return skip_help(object, item, static_cast<serializer_type*>(nullptr));
  }

  // TODO: set output to empty string.
  status serialize_to_string(const meta_object& object, std::string* output,
                             format_context* context) const override {
    if (std::is_convertible_v<T*, meta_object*>) {
      return serialize_to_string(
          object, meta_item_self_wrapper<T, std::is_convertible_v<T*, meta_object*>>(), output,
          context);
    } else {
      return status::error("should not run here!");
    }
  }

  status serialize_to_string(const meta_object& object, const meta_item& meta_item,
                             std::string* output, format_context* context) const override {
    return serializer_type::serialize_to_string(meta_item.get<T>(&object), output, context);
  }

  status serialize_from_string(const std::string_view& str_value, meta_object* object,
                               const meta_item& meta_item) const override {
    return serializer_type::serialize_from_string(str_value, meta_item.mutable_get<T>(object));
  }

  status serialize_from_string(const std::string_view& value, meta_object* object) const override {
    if (std::is_convertible_v<T*, meta_object*>) {
      return serialize_from_string(
          value, object, meta_item_self_wrapper<T, std::is_convertible_v<T*, meta_object*>>());
    } else {
      return status::error("should not run here!");
    }
  }

  status serialize_to_bytes(const meta_object& object, std::vector<char>* out,
                            endian endian_v) const override {
    if (std::is_convertible_v<T*, meta_object*>) {
      return serialize_to_bytes(
          object, meta_item_self_wrapper<T, std::is_convertible_v<T*, meta_object*>>(), out,
          endian_v);
    } else {
      return status::error("should not run here!");
    }
  }

  status serialize_to_bytes(const meta_object& object, const meta_item& item,
                            std::vector<char>* out, endian endian_v) const override {
    return serializer_type::serialize_to_bytes(item.get<T>(&object), endian_v, out);
  }

  status serialize_from_bytes(const char* first, const char* second, meta_object* object,
                              endian endian_v) const override {
    if (std::is_convertible_v<T*, meta_object*>) {
      return serialize_from_bytes(
          first, second, object,
          meta_item_self_wrapper<T, std::is_convertible_v<T*, meta_object*>>(), endian_v);
    } else {
      return status::error("should not run here!");
    }
  }

  status serialize_from_bytes(const char* first, const char* last, meta_object* object,
                              const meta_item& item, endian endian_v) const override {
    return serializer_type::serialize_from_bytes(first, last, endian_v,
                                                 item.mutable_get<T>(object));
  }

private:
  // If serailizer_type define the skip function, will use this version.
  template <class SerializerT>
  std::enable_if_t<std::is_function_v<decltype(SerializerT::skip)>, bool>
  skip_help(const meta_object& object, const meta_item& item, SerializerT*) const {
    return SerializerT::skip(item.get<T>(&object));
  }

  bool skip_help(const meta_object& object, const meta_item& item, void*) const {
    return serializer_base::skip(object, item);
  };
};

} // namespace reflection
