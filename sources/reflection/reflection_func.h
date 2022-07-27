#pragma once

#include "common.h"
#include "func/func_base_classes.h"
#include "func/func_info_spec.h"
#include "func/func_item_spec.h"
#include "meta/meta_base_classes.h"

#define _reflect_func_item(class, name)                                                            \
  _namespace func_item_spec<class, decltype(&class ::name)>

#define _func_name(class, name) #name, &class ::name

#define reflected_func(class, ...)                                                                 \
  inline const _namespace func_info& get_func_info() const override {                              \
    return static_get_func_info();                                                                 \
  }                                                                                                \
  inline static const _namespace func_info& static_get_func_info() {                               \
    static _namespace                                                                              \
        func_info_spec<class, _func_fn(_reflect_func_item, _comma, class, __VA_ARGS__)>  \
            inst(#class, _func_fn(_func_name, _comma, class, __VA_ARGS__));         \
    return inst;                                                                                   \
  }

/* Example
struct Person : public meta_object {
  int general_func(int i);
  int const_func(int i) const;

  reflected_func(Person, (general_func), (const_func));
};
*/

