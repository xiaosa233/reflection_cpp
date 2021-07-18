#include "serialization/binary_coder.h"

#include "gtest/gtest.h"

namespace reflection {

TEST(BinaryCoderTest, code) {
  {
    std::vector<char> out;
    int32_t a = 0x01020304;
    expect_ok(binary_coder<int32_t>::code(a, endian::big, &out));
    EXPECT_EQ(4, out.size());
    EXPECT_EQ(0x01, out[0]);
    EXPECT_EQ(0x02, out[1]);
    EXPECT_EQ(0x03, out[2]);
    EXPECT_EQ(0x04, out[3]);

    out.clear();
    expect_ok(binary_coder<int32_t>::code(a, endian::little, &out));
    EXPECT_EQ(4, out.size());
    EXPECT_EQ(0x04, out[0]);
    EXPECT_EQ(0x03, out[1]);
    EXPECT_EQ(0x02, out[2]);
    EXPECT_EQ(0x01, out[3]);
  }

  {
    std::vector<char> out;
    std::vector<uint16_t> a = {0x010A, 0x020B, 0x030C};
    expect_ok(binary_coder<std::vector<uint16_t>>::code(a, endian::big, &out));
    EXPECT_EQ(6, out.size());
    EXPECT_EQ(0x01, out[0]);
    EXPECT_EQ(0x0A, out[1]);
    EXPECT_EQ(0x02, out[2]);
    EXPECT_EQ(0x0B, out[3]);
    EXPECT_EQ(0x03, out[4]);
    EXPECT_EQ(0x0C, out[5]);

    out.clear();
    expect_ok(binary_coder<std::vector<uint16_t>>::code(a, endian::little, &out));
    EXPECT_EQ(6, out.size());
    EXPECT_EQ(0x0A, out[0]);
    EXPECT_EQ(0x01, out[1]);
    EXPECT_EQ(0x0B, out[2]);
    EXPECT_EQ(0x02, out[3]);
    EXPECT_EQ(0x0C, out[4]);
    EXPECT_EQ(0x03, out[5]);
  }

  {
    std::vector<char> out;
    std::string a = "test_binary_coding";
    expect_ok(binary_coder<std::string>::code(a, endian::big, &out));
    EXPECT_EQ(a, std::string_view(out.data(), out.size()));
    out.clear();
    expect_ok(binary_coder<std::string>::code(a, endian::little, &out));
    EXPECT_EQ(a, std::string_view(out.data(), out.size()));
  }
}

TEST(BinaryCoderTest, decode) {
  {
    const std::vector<char> buffer = {0x01, 0x02, 0x03, 0x04};
    int inst = 0;
    expect_ok(binary_coder<int>::decode(buffer.data(), buffer.data() + buffer.size(), endian::big,
                                        &inst));
    EXPECT_EQ(0x01020304, inst);
  }
  {
    std::string_view a = "test_binary_decoding";
    std::string inst;
    expect_ok(binary_coder<std::string>::decode(a.data(), a.data() + a.size(), endian::big, &inst));
    EXPECT_EQ(a, inst);
  }
}

} // namespace reflection
