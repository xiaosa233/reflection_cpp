
#pragma once

#include <type_traits>
#include <vector>

#include "serialization/byte_swapper.h"
#include "serialization/endian.h"
#include "status.h"

namespace reflection {

template <class T>
struct binary_coder {
  static_assert(std::is_trivial_v<T>, " T must be a trival type");
  static status code(const T& inst, endian endian_v, std::vector<char>* out) {
    size_t prev_size = out->size();
    out->resize(out->size() + sizeof(T));
    memcpy(out->data() + prev_size, &inst, sizeof(T));
    if (endian_v != endian::native) {
      byte_swapper<sizeof(T)>::swap(out->data() + prev_size);
    }
    return status::ok();
  }

  static status decode(const char* begin, const char* end, endian endian_v, T* inst) {
    if (end - begin == sizeof(T)) {
      memcpy(inst, begin, sizeof(T));
      if (endian_v != endian::native) {
        byte_swapper<sizeof(T)>::swap(inst);
      }
      return status::ok();
    }
    return status::error("Size not match " + std::to_string(sizeof(T)));
  }
};

template <>
struct binary_coder<std::string> {
  static status code(const std::string& inst, endian endian_v, std::vector<char>* out) {
    size_t prev_size = out->size();
    out->resize(out->size() + inst.size());
    memcpy(out->data() + prev_size, inst.data(), inst.size());
    return status::ok();
  }

  static status decode(const char* begin, const char* end, endian endian_v, std::string* inst) {
    inst->assign(begin, end - begin);
    return status::ok();
  }
};

template <class T, class... Args>
struct binary_coder<std::vector<T, Args...>> {
  using vector_type = std::vector<T, Args...>;
  static status code(const vector_type& inst, endian endian_v, std::vector<char>* out) {
    static_assert(std::is_trivial_v<T>, "T must be a trival type");
    size_t prev_size = out->size();
    out->resize(out->size() + inst.size() * sizeof(T));
    memcpy(out->data() + prev_size, inst.data(), out->size() - prev_size);
    if (endian_v != endian::native) {
      for (char* char_ptr = out->data() + prev_size; char_ptr < out->data() + out->size();
           char_ptr += sizeof(T)) {
        byte_swapper<sizeof(T)>::swap(char_ptr);
      }
    }
    return status::ok();
  }

  static status decode(const char* begin, const char* end, endian endian_v, vector_type* inst) {
    static_assert(std::is_trivial_v<T>, "T must be a trival type");
    inst->clear();
    const ptrdiff_t buffer_size = end - begin;
    const size_t length = (buffer_size) / sizeof(T);
    if (length * sizeof(T) != (buffer_size)) {
      return status::error("Length of the data is not align!");
    }
    inst->resize(length);
    memcpy(inst->data(), begin, buffer_size);
    if (endian_v != endian::native) {
      for (auto& it : *inst) {
        byte_swapper<sizeof(T)>::swap(&it);
      }
    }
    return status::ok();
  }
};

} // namespace reflection