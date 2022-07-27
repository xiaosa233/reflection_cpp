
#include "byte_swapper.h"

#include "gtest/gtest.h"

namespace reflection {

TEST(ByteSwapperTest, swap) {
  {
    uint16_t value = 0x0102;
    byte_swapper<sizeof(uint16_t)>::swap(&value);
    uint16_t expect_value = 0x0201;
    EXPECT_EQ(expect_value, value);
  }
  {
    uint32_t value = 0x01020304;
    byte_swapper<sizeof(uint32_t)>::swap(&value);
    uint32_t expect_value = 0x04030201;
    EXPECT_EQ(expect_value, value);
  }
  {
    uint64_t value = 0x0102030405060708;
    byte_swapper<sizeof(uint64_t)>::swap(&value);
    uint64_t expect_value = 0x0807060504030201;
    EXPECT_EQ(expect_value, value);
    EXPECT_EQ(expect_value, value);
  }
}

} // namespace reflection