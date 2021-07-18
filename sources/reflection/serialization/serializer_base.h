#pragma once

#include <string>

#include "meta/meta_base_classes.h"
#include "serialization/endian.h"
#include "status.h"

namespace reflection {

class serializer_base {
public:
  virtual ~serializer_base() = default;

  // These two version serialize function we need because when serialize from an object,
  // We can not get the type info from them. It may be a meta_object inherited class, it just a
  // normal type .e.g. int, double, string.

  // If should skip to serialize
  virtual bool skip(const meta_object& object, const meta_item& item) const { return false; }

  // Serialize the object it self.
  // Actually, its can be a non-vritual function. But there are some tricky dependences.
  virtual status serialize_to_string(const meta_object& object, std::string* output, format_context* context) const {
    return status::error("None-implementation!");
  }

  // object is the instance which contains it.
  virtual status serialize_to_string(const meta_object& object, const meta_item& item,
                                     std::string* output, format_context* context) const {
    return status::error("None-implementation!");
  }

  virtual status serialize_from_string(const std::string_view& str_value,
                                       meta_object* object) const {
    return status::error("None-implementation!");
  }

  virtual status serialize_from_string(const std::string_view& str_value, meta_object* object,
                                       const meta_item& item) const {
    return status::error("None-implementation!");
  }

  virtual status serialize_to_bytes(const meta_object& object, std::vector<char>* out,
                                    endian endian_v = endian::native) const {
    return status::error("None-implementation!");
  }

  virtual status serialize_to_bytes(const meta_object& object, const meta_item& item,
                                    std::vector<char>* out,
                                    endian endian_v = endian::native) const {
    return status::error("None-implementation!");
  }

  virtual status serialize_from_bytes(const char* first, const char* last, meta_object* object,
                                      endian endian_v = endian::native) const {
    return status::error("None-implementation!");
  }
  virtual status serialize_from_bytes(const char* first, const char* last, meta_object* object,
                                      const meta_item& item,
                                      endian endian_v = endian::native) const {
    return status::error("None-implementation!");
  }
};

template <class T>
struct serializer;

} // namespace reflection
