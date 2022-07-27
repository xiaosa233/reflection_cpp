#pragma once

#include <memory>
#include <unordered_map>

#include "common.h"
#include "meta/meta_base_classes.h"

namespace reflection {

template <class T, class... Args>
struct meta_info_spec : public meta_info {
  static_assert(std::is_convertible_v<T*, meta_object*>, " T is not a meta_object derived class!");
  using type = std::tuple<std::unique_ptr<Args>...>;

  template <class... ParamArgs>
  meta_info_spec(const char* class_name, ParamArgs&&... args) : meta_info(class_name) {
    Initializer<0, std::tuple_size_v<type>>::work(*this, std::forward<ParamArgs>(args)...);
  }

  ~meta_info_spec() override = default;

  std::unique_ptr<meta_object> create() const override { return std::make_unique<T>(); }

private:
  template <size_t I, size_t N>
  struct Initializer {
    template <class... ParamArgs>
    static void work(meta_info_spec& meta_info, const char* type_name, const char* name,
                     ParamArgs&&... args) {
      std::get<I>(meta_info.meta_items) =
          std::make_unique<typename std::tuple_element_t<I, meta_info_spec::type>::element_type>(
              type_name, name);
      meta_info.items_.insert({name, std::get<I>(meta_info.meta_items).get()});
      meta_info.item_vec_.push_back(std::get<I>(meta_info.meta_items).get());

      Initializer<I + 1, N>::work(meta_info, std::forward<ParamArgs>(args)...);
    }
  };
  template <size_t N>
  struct Initializer<N, N> {
    static void work(meta_info_spec& meta_info) {}
  };

  type meta_items;
};

} // namespace reflection
