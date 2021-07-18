#pragma once

#include <cstdint>

namespace reflection {

template <size_t N>
struct byte_swapper {
  inline static void swap(void* ptr) { static_assert(false, "Can not swap size with N"); }
};

template <>
struct byte_swapper<1> {
  inline static void swap(void* ptr) {
    // Do nothing.
    (void)(ptr);
  }
};

template <>
struct byte_swapper<2> {
  inline static void swap(void* ptr) {
    uint16_t& value = *reinterpret_cast<uint16_t*>(ptr);
    value = ((value << 8) & 0xFF00) | ((value >> 8) & 0xFF);
  }
};

template <>
struct byte_swapper<4> {
  inline static void swap(void* ptr) {
    uint32_t& value = *reinterpret_cast<uint32_t*>(ptr);
    value = ((value & 0xFF) << 24) | ((value & 0xFF00) << 8) | ((value & 0xFF0000) >> 8) |
            ((value >> 24) & 0xFF);
  }
};

template <>
struct byte_swapper<8> {
  static void swap(void* ptr) {
    uint64_t& value = *reinterpret_cast<uint64_t*>(ptr);
    value = ((value & 0xFF) << 56) | ((value & 0xFF00) << 40) | ((value & 0xFF0000) << 24) |
            ((value & 0xFF000000) << 8) | ((value & 0xFF00000000) >> 8) |
            ((value >> 24) & 0xFF0000) | ((value >> 40) & 0xFF00) | ((value >> 56) & 0xFF);
  }
};

} // namespace reflection