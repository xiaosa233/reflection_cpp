#pragma once

#include "func/func_item_args.h"
#include "func/string_callable.h"

namespace reflection {

// Class for function item. Contains the function pointer.
template <class ClassT, class MemberFuncPtr>
class func_item_spec : public func_item_trait_args<ClassT, MemberFuncPtr> {
public:
  using member_func_info_t = member_func_info<MemberFuncPtr>;
  using return_type = typename member_func_info_t::return_type;
  static constexpr bool is_static_func = member_func_info_t::is_static_func;
  static constexpr bool constexpr_is_const = member_func_info_t::is_const;
  static constexpr size_t constexpr_args_size = member_func_info_t::args_size;
  using class_pointer = std::conditional_t<constexpr_is_const, const ClassT*, ClassT*>;

  func_item_spec(const char* func_name, MemberFuncPtr ptr)
      : func_item_trait_args(func_name), ptr_(ptr),
        func_signature_(
            trait_signature_helper<typename member_func_info_t::functional_type>::trait()) {}

  bool is_static() const override { return is_static_func; }
  bool is_const() const override { return constexpr_is_const; }
  size_t args_size() const override { return constexpr_args_size; }

  string_status invoke_by_string(const std::vector<std::string>& args,
                                 meta_object* object) const override {
    return invoke_by_string_help<member_func_info_t::is_serializable>(args, object);
  }
  bool is_serializable() const override { return member_func_info_t::is_serializable; }

  const func_signature& get_signature() const override { return func_signature_; }

  template <class... Args>
  context_status<return_type> invoke(Args&&... args) const {
    return invoke_help<is_static_func, std::is_void_v<return_type>>(std::forward<Args>(args)...);
  }

private:
  // static, void
  template <bool static_func, bool is_void_return,
            std::enable_if_t<static_func && is_void_return, bool> = true, class... Args>
  context_status<return_type> invoke_help(Args&&... args) const {
    (*ptr_)(std::forward<Args>(args)...);
    return {};
  }

  // static, none-void
  template <bool static_func, bool is_void_return,
            std::enable_if_t<static_func && !is_void_return, bool> = true, class... Args>
  context_status<return_type> invoke_help(Args&&... args) const {
    return context_status<return_type>::ok((*ptr_)(std::forward<Args>(args)...));
  }

  // none-static, void
  template <bool static_func, bool is_void_return,
            std::enable_if_t<!static_func && is_void_return, bool> = true, class... Args>
  context_status<return_type> invoke_help(class_pointer inst, Args&&... args) const {
    (inst->*ptr_)(std::forward<Args>(args)...);
    return {};
  }

  // none-static, none-void
  template <bool static_func, bool is_void_return,
            std::enable_if_t<!static_func && !is_void_return, bool> = true, class... Args>
  context_status<return_type> invoke_help(class_pointer inst, Args&&... args) const {
    return context_status<return_type>::ok((inst->*ptr_)(std::forward<Args>(args)...));
  }

  // If functional type can not be serialized to the string, we will just return error.
  template <bool is_serializable>
  string_status invoke_by_string_help(const std::vector<std::string>& args,
                                      meta_object* object) const {
    static_assert(std::is_convertible_v<ClassT*, meta_object*>,
                  "Class type must be the devied class of the meta_object");
    if (args.size() != constexpr_args_size) {
      return string_status::error(
          get_func_hint() + ": Size of args is not equal with the real size. " +
          std::to_string(args.size()) + " vs " + std::to_string(constexpr_args_size));
    }
    return string_status::ok(string_callable<constexpr_args_size>::call(
        create_member_function_wrapper(
            ptr_, static_cast<std::conditional_t<is_static_func, void*, ClassT*>>(object)),
        args));
  }
  template <>
  string_status invoke_by_string_help<false>(const std::vector<std::string>& args,
                                             meta_object* object) const {
    return string_status::error(get_func_hint() +
                                " return type or args can not serialize to the string!");
  }

  MemberFuncPtr ptr_;
  func_signature func_signature_;
};

}