#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "common.h"
#include "func/func_signature.h"
#include "to_string_utils.h"

namespace reflection {

class meta_object;

template <class ReturnT, class... Args>
class func_item_args_type;

class func_item {
public:
  func_item(const char* func_name) : func_name_(func_name){};

  virtual ~func_item() = default;

  const char* func_name() const { return func_name_; }

  virtual bool is_static() const = 0;
  virtual bool is_const() const = 0;
  virtual size_t args_size() const = 0;

  // Only when true is returned, invoke_by_string is available
  virtual bool is_serializable() const = 0;
  virtual const func_signature& get_signature() const = 0;

  string_status invoke_by_string(const std::vector<std::string>& args,
                                 const meta_object* object = nullptr) const {
    // It is impossible to statically assert whether this function is const,
    // so this function will forward to non-const functions.
    // Runtime checks are done in subclasses.
    return invoke_by_string(args, const_cast<meta_object*>(object));
  }

  // If object is nullptr, it will explain to call by static member function
  virtual string_status invoke_by_string(const std::vector<std::string>& args,
                                         meta_object* object) const = 0;

#pragma optimize("s", on)
  template <class return_type = void, class... Args>
  context_status<return_type> invoke(Args&&... args) const {
    constexpr size_t call_args_size = sizeof...(Args);
    if (!is_static() && call_args_size == 0) {
      return context_status<return_type>::error(get_func_hint() + ": miss callable object inst!");
    }
    func_signature call_signature = trait_signature_helper<return_type(Args...)>::trait();
    if (!is_static() && args_size() + 1 != call_args_size) {
      return context_status<return_type>::error(
          get_func_hint() +
          ": count of args size is not eual, may be miss callable object inst! invoke info is " +
          call_signature.to_string());
    }
    if (!is_static()) {
      call_signature.args_type.erase(call_signature.args_type.begin());
    }
    const func_signature& real_signature = get_signature();
// weird trick. If enable optimization, somehow, it will go into the failure branch whatever the return value.
    if (!call_signature.to(real_signature)) {
      return context_status<return_type>::error(real_signature.to_string() + ": error invoke info " +
                                                call_signature.to_string());
    }
    return static_cast<const func_item_args_type<return_type, Args...>*>(this)->invoke_help(
        std::forward<Args>(args)...);
  }
#pragma optimize("s", off)

protected:
  std::string get_func_hint() const {
    return std::string(func_name()) + " " + get_signature().to_string();
  }

  const char* func_name_ = nullptr;
};

class func_info {
public:
  func_info(const char* class_name) : class_name_(class_name) {}
  virtual ~func_info() = default;

  std::string class_name() const { return class_name_; }

  inline const std::unordered_map<std::string, const func_item*>& func_item_map() const {
    return func_items_;
  }

  bool has(const std::string_view& name) const { return func_items_.count(view_to_string(name)); }

  const func_item& get_func_item(const std::string& name) const { return *func_items_.at(name); }

protected:
  const char* class_name_;
  std::unordered_map<std::string, const func_item*> func_items_;

  DISALLOW_COPY_AND_MOVE(func_info);
};

} // namespace reflection