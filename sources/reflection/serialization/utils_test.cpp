#include "gtest/gtest.h"

#include "utils.h"

namespace reflection {

TEST(UtilsTest, startswith) {
  EXPECT_TRUE(startswith("123 adf", "1"));
  EXPECT_TRUE(startswith("123", "123"));
  EXPECT_TRUE(startswith("123 adf", "12"));
  EXPECT_TRUE(startswith("123 adf", "123 "));
  EXPECT_TRUE(startswith("123 adf", ""));
  EXPECT_FALSE(startswith("123 adf", "abc"));
}

TEST(UtilsTest, get_token) {
  std::string_view input_base = "{    size: 233, person_list: [a,b] ,str: \"hello \\\"world!\"}";
  std::string_view input = input_base;
  EXPECT_EQ("{", get_token(&input));
  EXPECT_EQ("    size: 233, person_list: [a,b] ,str: \"hello \\\"world!\"}", input);
  EXPECT_EQ("size", get_token(&input));
  EXPECT_EQ(":", get_token(&input));
  EXPECT_EQ("233", get_token(&input));
  EXPECT_EQ(",", get_token(&input));
  EXPECT_EQ("person_list", get_token(&input));
  EXPECT_EQ(":", get_token(&input));
  EXPECT_EQ("[", get_token(&input));
  EXPECT_EQ("a", get_token(&input));
  EXPECT_EQ(",", get_token(&input));
  EXPECT_EQ("b", get_token(&input));
  EXPECT_EQ("]", get_token(&input));
  EXPECT_EQ(",", get_token(&input));
  EXPECT_EQ("str", get_token(&input));
  EXPECT_EQ(":", get_token(&input));
  EXPECT_EQ("\"hello \\\"world!\"", get_token(&input));
  EXPECT_EQ("}", get_token(&input));

  input = input_base.substr(2, 0);
  EXPECT_TRUE(get_token(&input).empty());
}

TEST(UtilsTest, is_world) {
  EXPECT_TRUE(is_world("jack"));
  EXPECT_TRUE(is_world("123"));
  EXPECT_TRUE(is_world("0.123"));
  EXPECT_TRUE(is_world("a"));
  EXPECT_TRUE(is_world("_asd"));

  EXPECT_FALSE(is_world("["));
  EXPECT_FALSE(is_world("]"));
  EXPECT_FALSE(is_world("{"));
  EXPECT_FALSE(is_world("}"));
  EXPECT_FALSE(is_world(":"));
  EXPECT_FALSE(is_world(","));
}

TEST(UtilsTest, skip_name) {
  std::string_view input = " id :jack";
  EXPECT_TRUE(skip_name("id", &input));
  EXPECT_EQ("jack", input);

  input = " jack ";
  EXPECT_TRUE(skip_name("", &input));
  EXPECT_FALSE(skip_name("id", &input));
}

TEST(UtilsTest, get_word) {
  {
  std::string_view input = "id:jack";
  std::string_view output_world;
  EXPECT_TRUE(get_word(&input, &output_world));
  EXPECT_EQ("id", output_world);
  EXPECT_EQ(":jack", input);

  EXPECT_FALSE(get_word(&input, &output_world));
  }
  {
    std::string_view input = "str:\"hello world!\"";
    std::string_view output_world;
    EXPECT_TRUE(get_word(&input, &output_world));
    EXPECT_EQ("str", output_world);
    EXPECT_EQ(":\"hello world!\"", input);
    input = input.substr(1);
    EXPECT_TRUE(get_word(&input, &output_world));
    EXPECT_EQ("\"hello world!\"", output_world);
    EXPECT_EQ("", input);
  }
}

TEST(UtilsTest, get_variable_part) {
  std::string_view input = " 0.123 { id:jack, {str_value:\"a\\\"sd f}\"}} [1,2,3]";
  std::string_view output;
  expect_ok(get_variable_part(&input, &output));
  EXPECT_EQ("0.123", output);
  expect_ok(get_variable_part(&input, &output));
  EXPECT_EQ("{ id:jack, {str_value:\"a\\\"sd f}\"}}", output);
  expect_ok(get_variable_part(&input, &output));
  EXPECT_EQ("[1,2,3]", output);

  input = ",asdf";
  EXPECT_FALSE(get_variable_part(&input, &output));
  EXPECT_EQ(",asdf", input);
};

} // namespace reflection
