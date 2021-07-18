#include "text_coder.h"

#include "gtest/gtest.h"

#include "utils.h"

namespace reflection {

TEST(TextCoderTest, TextCoder) {
  // bool
  {
    bool a;
    EXPECT_TRUE(text_coder<bool>::decode("true", &a));
    EXPECT_TRUE(a);
    std::string result;
    EXPECT_TRUE(text_coder<bool>::code(a, &result));
    EXPECT_EQ("true", result);
    result.clear();
    EXPECT_TRUE(text_coder<bool>::decode("false", &a));
    EXPECT_FALSE(a);
    EXPECT_TRUE(text_coder<bool>::code(a, &result));
    EXPECT_EQ("false", result);
    EXPECT_FALSE(text_coder<bool>::decode("1", &a));
  }

  // char
  {
    char a;
    EXPECT_TRUE(text_coder<char>::decode("a", &a));
    EXPECT_EQ('a', a);
    std::string result;
    EXPECT_TRUE(text_coder<char>::code(a, &result));
    EXPECT_EQ("a", result);
  }

  // string
  {
    std::string a;
    EXPECT_TRUE(text_coder<std::string>::decode("\"a_string\"", &a));
    EXPECT_EQ("a_string", a);
    std::string result;
    EXPECT_TRUE(text_coder<std::string>::code(a, &result));
    EXPECT_EQ("\"a_string\"", result);
  }

  {
    int32_t a;
    EXPECT_TRUE(text_coder<int32_t>::decode("123", &a));
    EXPECT_EQ(123, a);
    std::string result;
    EXPECT_TRUE(text_coder<int32_t>::code(a, &result));
    EXPECT_EQ("123", result);
  }
  {
    uint32_t a;
    EXPECT_TRUE(text_coder<uint32_t>::decode("-1", &a));
    EXPECT_EQ(std::numeric_limits<uint32_t>::max(), a);
    std::string result;
    EXPECT_TRUE(text_coder<uint32_t>::code(a, &result));
    EXPECT_EQ("4294967295", result);
  }
  {
    int64_t a;
    EXPECT_TRUE(text_coder<int64_t>::decode("-9223372036854775808", &a));
    EXPECT_EQ(-9223372036854775808, a);
    std::string result;
    EXPECT_TRUE(text_coder<int64_t>::code(a, &result));
    EXPECT_EQ("-9223372036854775808", result);
  }
  {
    uint64_t a;
    EXPECT_TRUE(text_coder<uint64_t>::decode("18446744073709551615", &a));
    EXPECT_EQ(std::numeric_limits<uint64_t>::max(), a);
    std::string result;
    EXPECT_TRUE(text_coder<uint64_t>::code(a, &result));
    EXPECT_EQ("18446744073709551615", result);
  }

  // float type
  {
    float a;
    EXPECT_TRUE(text_coder<float>::decode("0.1234567", &a));
    EXPECT_EQ(0.1234567f, a);
    std::string result;
    EXPECT_TRUE(text_coder<float>::code(a, &result));
    EXPECT_TRUE(startswith(result, "0.1234567"));
  }

  {
    double a;
    EXPECT_TRUE(text_coder<double>::decode("0.12345678901234", &a));
    EXPECT_EQ(0.12345678901234, a);
    std::string result;
    EXPECT_TRUE(text_coder<double>::code(a, &result));
    EXPECT_TRUE(startswith(result, "0.12345678901234"));
  }
}

} // namespace reflection