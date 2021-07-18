#pragma once

namespace reflection {

enum class endian { little = 0, big = 1, native };

endian get_machine_endian();

endian get_opposite_endian(endian endian_v);

inline endian get_real_endian(endian endian_v) {
  return get_opposite_endian(get_opposite_endian(endian_v));
}

} // namespace reflection

bool operator==(reflection::endian lhs, reflection::endian rhs);

inline bool operator!=(reflection::endian lhs, reflection::endian rhs) {
  return !(lhs == rhs);
}
