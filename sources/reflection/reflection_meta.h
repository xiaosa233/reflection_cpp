#pragma once

#include "common.h"
#include "meta/meta_base_classes.h"
#include "meta/meta_info_spec.h"
#include "meta/meta_item_spec_serializer.h"
#include "meta/meta_manager.h"

#define _reflect_declare(class, type, name, ...) type name __VA_ARGS__

#define _reflect_meta_item(class, type, name) _namespace meta_item_spec<class, type, &class ::name>

#define _reflect_string(class, type, name) #type, #name

// Count of the parameters can not be 0.
#define reflected(class, ...)                                                                      \
  _func_fn(_reflect_declare, _semicolon, class, __VA_ARGS__);                                      \
  inline const _namespace meta_info& get_meta_info() const override {                              \
    return static_get_meta_info();                                                                 \
  }                                                                                                \
  inline static const _namespace meta_info& static_get_meta_info() {                               \
    static _namespace                                                                              \
        meta_info_spec<class, _func_fn(_reflect_meta_item, _comma, class, _convert(__VA_ARGS__))>  \
            inst(#class, _func_fn(_reflect_string, _comma, class, _convert(__VA_ARGS__)));         \
    return inst;                                                                                   \
  }                                                                                                \
  inline const _namespace serializer_base& get_serializer() const override {                       \
    static _namespace serializer<class> inst;                                                      \
    return inst;                                                                                   \
  }

/* Example
struct Person : public meta_object {
  reflected(Person, (int, id, = 3), (std::string, name));
  // equal to
  // int id=3;
  // std::string name;
};
*/

