
#include "endian.h"

#include "gtest/gtest.h"

namespace reflection {

TEST(EndianTest, endian) {
  EXPECT_TRUE(endian::big == endian::big);
  EXPECT_TRUE(endian::little == endian::little);
  EXPECT_TRUE(endian::little == get_opposite_endian(endian::big));
  EXPECT_TRUE(endian::big == get_opposite_endian(endian::little));
  if (get_machine_endian() == endian::little) {
    EXPECT_TRUE(endian::big == get_opposite_endian(endian::native));
    EXPECT_TRUE(endian::native == endian::little);
  } else {
    EXPECT_TRUE(endian::little == get_opposite_endian(endian::native));
    EXPECT_TRUE(endian::native == endian::big);
  }
}

} // namespace reflection
