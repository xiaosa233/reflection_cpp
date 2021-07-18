#pragma once

#include "func/func_base_classes.h"

namespace reflection {

template <class T, class... Args>
struct func_info_spec : public func_info {
  static_assert(std::is_convertible_v<T*, meta_object*>, " T is not a meta_object derived class!");
  using type = std::tuple<std::unique_ptr<Args>...>;

  template <class... ParamArgs>
  func_info_spec(const char* class_name, ParamArgs&&... args) : func_info(class_name) {
    Initializer<0, std::tuple_size_v<type>>::work(*this, std::forward<ParamArgs>(args)...);
  }

  ~func_info_spec() override = default;

private:
  template <size_t I, size_t N>
  struct Initializer {
    template <class MemberFuncPtr, class... ParamArgs>
    static void work(func_info_spec& func_info, const char* name, MemberFuncPtr ptr,
                     ParamArgs&&... args) {
      std::get<I>(func_info.func_items) =
          std::make_unique<typename std::tuple_element_t<I, func_info_spec::type>::element_type>(
              name, ptr);
      func_info.func_items_.insert({name, std::get<I>(func_info.func_items).get()});

      Initializer<I + 1, N>::work(func_info, std::forward<ParamArgs>(args)...);
    }
  };
  template <size_t N>
  struct Initializer<N, N> {
    static void work(func_info_spec& func_info) {}
  };

  type func_items;
};

} // namespace reflection
