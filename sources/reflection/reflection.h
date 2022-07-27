#pragma once

#include "reflection_func.h"
#include "reflection_meta.h"

// Use this when there is no variables to reflect.
#define reflected_empty(class)                                                                     \
  inline const _namespace meta_info& get_meta_info() const override {                              \
    return static_get_meta_info();                                                                 \
  }                                                                                                \
  inline static const _namespace meta_info& static_get_meta_info() {                               \
    static _namespace meta_info_spec<class> inst(#class);                                          \
    return inst;                                                                                   \
  }                                                                                                \
  inline const _namespace serializer_base& get_serializer() const override {                       \
    static _namespace serializer<class> inst;                                                      \
    return inst;                                                                                   \
  }                                                                                                \
  inline const _namespace func_info& get_func_info() const override {                              \
    return static_get_func_info();                                                                 \
  }                                                                                                \
  inline static const _namespace func_info& static_get_func_info() {                               \
    static _namespace func_info_spec<class> inst(#class);                                          \
    return inst;                                                                                   \
  }

#define register_to_meta_manager(class)                                                            \
  const _namespace meta_info& _##class##_meta_info_inst = class ::static_get_meta_info();          \

/* Example
struct Person : public meta_object {
  reflected(Person, (int, id, = 3), (std::string, name));
  // equal to
  // int id=3;
  // std::string name;
};

register_to_meta_manager(Person);
*/
