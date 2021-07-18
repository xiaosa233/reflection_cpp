#include "endian.h"

#include <cstdint>

namespace reflection {

namespace {
endian machine_endian() {
  int32_t int_value = 0x1;
  return *(char*)(&int_value) == 1 ? endian::little : endian::big;
}

endian mechine_endian_v = machine_endian();

} // namespace

endian get_machine_endian() {
  return mechine_endian_v;
}

endian get_opposite_endian(endian endain_v) {
  if (endain_v == endian::native) {
    endain_v = get_machine_endian();
  }
  return endain_v == endian::little ? endian::big : endian::little;
}

} // namespace reflection

bool operator==(reflection::endian lhs, reflection::endian rhs) {
  return static_cast<int>((static_cast<int>(lhs) == static_cast<int>(reflection::endian::native)
                               ? reflection::get_machine_endian()
                               : lhs)) ==
         static_cast<int>((static_cast<int>(rhs) == static_cast<int>(reflection::endian::native)
                               ? reflection::get_machine_endian()
                               : rhs));
}
