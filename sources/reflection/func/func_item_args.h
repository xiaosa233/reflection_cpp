#pragma once

#include "func/func_base_classes.h"
#include "serialization/trait_utils.h"

namespace reflection {

template <class T>
struct member_func_info {};

template <class T>
struct is_arg_serializable {
  static constexpr bool value =
      !std::is_reference_v<T> && (std::is_same_v<T, std::string> || std::is_void_v<T> || reflected_type<std::decay_t<T>>::value !=
                                      reflected_type<std::decay_t<T>>::type_enum::general ||
                                  std::is_arithmetic_v<T>);
};

template <class T>
struct is_arg_serializable<const T&> : public is_arg_serializable<T> {};

template <class T>
struct is_arg_serializable<T&&> : public is_arg_serializable<T> {};

template <class... Args>
struct is_functional_serializable {};

template <class R, class... Args>
struct is_functional_serializable<R(Args...)> {
  static constexpr bool value =
      is_arg_serializable<R>::value && (is_arg_serializable<Args>::value && ...);
};

template <class R, class T, class... Args>
struct member_func_info<R (T::*)(Args...)> {
  using return_type = R;
  using functional_type = R(Args...);
  using class_type = T;
  static constexpr bool is_static_func = false;
  static constexpr bool is_const = false;
  static constexpr size_t args_size = sizeof...(Args);
  static constexpr bool is_serializable = is_functional_serializable<functional_type>::value;
};

template <class R, class T, class... Args>
struct member_func_info<R (T::*)(Args...) const> {
  using return_type = R;
  using functional_type = R(Args...);
  using class_type = T;
  static constexpr bool is_static_func = false;
  static constexpr bool is_const = true;
  static constexpr size_t args_size = sizeof...(Args);
  static constexpr bool is_serializable = is_functional_serializable<functional_type>::value;
};

template <class R, class... Args>
struct member_func_info<R (*)(Args...)> {
  using return_type = R;
  using functional_type = R(Args...);
  static constexpr bool is_static_func = true;
  static constexpr bool is_const = false;
  static constexpr size_t args_size = sizeof...(Args);
  static constexpr bool is_serializable = is_functional_serializable<functional_type>::value;
};

template <class ReturnT, class... Args>
class func_item_args_type : public func_item {
public:
  func_item_args_type(const char* func_name) : func_item(func_name){};

  context_status<ReturnT> invoke_help(Args&&... args) const {
    return invoke_forward(std::forward<Args>(args)...);
  }

protected:
  virtual context_status<ReturnT> invoke_forward(Args&&... args) const = 0;
};

// Forward declare
template <class ClassT, class MemberFuncPtr>
class func_item_spec;

// It is used to get the type of the args.
template <class ClassT, class Func>
class func_item_trait_args {
  // template <class... Args>
  // func_item_trait_args(Args&&...) {
  //   static_assert(false, "Should not arrive.");
  // }
};

// For non-const member function
template <class ClassT, class R, class... Args>
class func_item_trait_args<ClassT, R (ClassT::*)(Args...)>
    : public func_item_args_type<R, ClassT*, Args...> {
public:
  using derived_item = func_item_spec<ClassT, R (ClassT::*)(Args...)>;
  func_item_trait_args(const char* func_name) : func_item_args_type<R, ClassT*, Args...>(func_name) {}

protected:
  virtual context_status<R> invoke_forward(ClassT*&& inst,
                                                     Args&&... args) const override {
    return static_cast<const derived_item*>(this)->invoke(std::forward<ClassT*>(inst),
                                                          std::forward<Args>(args)...);
  }
};

// For const member function
template <class ClassT, class R, class... Args>
class func_item_trait_args<ClassT, R (ClassT::*)(Args...) const>
    : public func_item_args_type<R, const ClassT*, Args...> {

public:
  using derived_item = func_item_spec<ClassT, R (ClassT::*)(Args...) const>;
  func_item_trait_args(const char* func_name) : func_item_args_type<R, const ClassT*, Args...>(func_name) {}

protected:
  virtual context_status<R> invoke_forward(const ClassT*&& inst,
                                                     Args&&... args) const override {
    return static_cast<const derived_item*>(this)->invoke(std::forward<const ClassT*>(inst),
                                                          std::forward<Args>(args)...);
  }
};

// For static member function
template <class ClassT, class R, class... Args>
class func_item_trait_args<ClassT, R (*)(Args...)> : public func_item_args_type<R, Args...> {
public:
  using derived_item = func_item_spec<ClassT, R (*)(Args...)>;
  func_item_trait_args(const char* func_name) : func_item_args_type<R, Args...>(func_name) {}

protected:
  virtual context_status<R> invoke_forward(Args&&... args) const override {
    return static_cast<const derived_item*>(this)->invoke(std::forward<Args>(args)...);
  }
};

} // namespace reflection
