#pragma once

#include <assert.h>

#include "meta/meta_base_classes.h"
#include "meta/meta_info_spec.h"
#include "meta/meta_item_spec_serializer.h"

namespace reflection {


// SomeMetaObejct inst = aggregate_initialize(...); order make sense.
template <class MetaT, class... Args>
MetaT aggregate_initialize(Args&&... args);

// SomeMetaObejct inst = pair_initialize({"name", ...},{},{});
template <class MetaT, class... Args>
MetaT pair_initialize(Args&&... args);

std::string object_to_string(const meta_object& object, format_context* context = nullptr);

status serialize_to_text_file(const meta_object& object, const std::string_view& file_path,
                              bool overwrite = true, format_context context = {});


// TODO: Get the error info of the column and row.
status serialize_from_text_file(const std::string_view& file_path, meta_object* object);

status serialize_to_binary_file(const meta_object& object, const std::string_view& file_path,
                                endian endian_v = endian::native, bool overwrite = true);

status serialize_from_binary_file(const std::string_view& file_path, meta_object* object);



// --------- Implementation ------------

template <class ItemIt, class T, class... Args>
void aggregate_initialize_help(ItemIt first, ItemIt last, meta_object* object, T&& t,
                               Args&&... args) {
  assert(first != last);
  object->template set<std::decay_t<T>>((*first)->name(), std::forward<T>(t));
  aggregate_initialize_help(std::next(first), last, object, std::forward<Args>(args)...);
}

template <class ItemIt>
void aggregate_initialize_help(ItemIt first, ItemIt last, meta_object* object) {
}

template <class MetaT, class... Args>
MetaT aggregate_initialize(Args&&... args) {
  MetaT object;
  aggregate_initialize_help(object.get_meta_info().item_vec().begin(),
                            object.get_meta_info().item_vec().end(), &object,
                            std::forward<Args>(args)...);
  return object;
}

template <class CharT, class MemT, class T>
void pair_initialize_help(std::pair<CharT, MemT>&& pair_v, T* object) {
  assert(object->has_meta(pair_v.first));
  object->template set<typename std::decay_t<MemT>>(pair_v.first, std::forward<MemT>(pair_v.second));
}

template <class MetaT, class... Args>
MetaT pair_initialize(Args&&... args) {
  MetaT object;
  int a[] = {(pair_initialize_help(std::forward<Args>(args), &object), 1)...};
  return object;
}

} // namespace reflection