#include "func/func_signature.h"

#include "gtest/gtest.h"

#include "reflection.h"

namespace reflection {

struct widget : public meta_object {

  void general();

  reflected(widget, (int, id, = 1));
  reflected_func(widget, (general));
};

namespace func_signature_detail {

constexpr char pointer_size() noexcept {
  return sizeof(void*) + '0';
}

std::string append_pointer(const std::string_view& str_view) {
  return std::string(str_view.begin(), str_view.end()) + pointer_size();
}

template <class T>
std::string append_size() {
  return std::to_string(sizeof(T));
}

template <class T, class U>
bool func_args_to_test() {
  return trait_args<T>().to(trait_args<U>());
}

TEST(TypeNameTest, get_decay_type) {
  EXPECT_EQ("object", get_decay_type("object&&"));
  EXPECT_EQ("object", get_decay_type("const object"));
  EXPECT_EQ("object", get_decay_type("object"));
  EXPECT_EQ("object", get_decay_type("object&"));
  EXPECT_EQ("object**", get_decay_type("object**"));
  EXPECT_EQ("object", get_decay_type("const object&"));
  EXPECT_EQ("object*", get_decay_type("const object*"));
  EXPECT_EQ("object*", get_decay_type("const object*const"));
  EXPECT_EQ("object*", get_decay_type("const object*&"));
}

TEST(TypeNameTest, type_name) {
  EXPECT_EQ("void_0", trait_args<void>().to_string());

  EXPECT_EQ("signed_integral_4", trait_args<int32_t>().to_string());
  EXPECT_EQ("unsigned_integral_4", trait_args<uint32_t>().to_string());
  EXPECT_EQ("signed_integral_8", trait_args<int64_t>().to_string());
  EXPECT_EQ("floating_point_4", trait_args<float>().to_string());
  EXPECT_EQ("floating_point_8", trait_args<double>().to_string());

  EXPECT_EQ("signed_integral&_4", trait_args<int32_t&>().to_string());
  EXPECT_EQ("signed_integral&&_4", trait_args<int32_t&&>().to_string());
  EXPECT_EQ("const signed_integral_4", trait_args<const int32_t>().to_string());
  EXPECT_EQ("const signed_integral&_4", trait_args<const int32_t&>().to_string());
  EXPECT_EQ(append_pointer("signed_integral*_"), trait_args<int32_t*>().to_string());
  EXPECT_EQ(append_pointer("signed_integral*&_"), trait_args<int32_t*&>().to_string());
  EXPECT_EQ(append_pointer("const signed_integral*_"), trait_args<const int32_t*>().to_string());
  EXPECT_EQ(append_pointer("const signed_integral*const_"), trait_args<const int32_t*const>().to_string());
  EXPECT_EQ(append_pointer("signed_integral**_"), trait_args<int32_t**>().to_string());
  EXPECT_EQ(append_pointer("const signed_integral*_"), trait_args<const int32_t*>().to_string());
  EXPECT_EQ(append_pointer("const signed_integral*const_"),
            trait_args<const int32_t* const>().to_string());

  EXPECT_EQ("meta_object_sub_class_" + append_size<widget>(), trait_args<widget>().to_string());
  EXPECT_EQ("unknown_" + append_size<std::vector<int>>(),
            trait_args<std::vector<int>>().to_string());

  {
    func_args arg = trait_args<int32_t>();
    EXPECT_FALSE(arg.is_const);
    EXPECT_FALSE(arg.is_lvalue_reference);
    EXPECT_FALSE(arg.is_rvalue_reference);
    EXPECT_FALSE(arg.is_const_pointer);
  }
  {
    func_args arg = trait_args<const int32_t&>();
    EXPECT_TRUE(arg.is_const);
    EXPECT_TRUE(arg.is_lvalue_reference);
    EXPECT_FALSE(arg.is_rvalue_reference);
    EXPECT_FALSE(arg.is_const_pointer);
  }
  {
    func_args arg = trait_args<const int32_t* const>();
    EXPECT_TRUE(arg.is_const);
    EXPECT_FALSE(arg.is_lvalue_reference);
    EXPECT_FALSE(arg.is_rvalue_reference);
    EXPECT_TRUE(arg.is_const_pointer);
  }
  {
    func_args arg = trait_args<const int32_t*>();
    EXPECT_FALSE(arg.is_const);
    EXPECT_FALSE(arg.is_lvalue_reference);
    EXPECT_FALSE(arg.is_rvalue_reference);
    EXPECT_TRUE(arg.is_const_pointer);
  }
  {
    func_args arg = trait_args<const int32_t*&>();
    EXPECT_FALSE(arg.is_const);
    EXPECT_TRUE(arg.is_lvalue_reference);
    EXPECT_FALSE(arg.is_rvalue_reference);
    EXPECT_TRUE(arg.is_const_pointer);
  }
  {
    func_args arg = trait_args<int32_t*const>();
    EXPECT_TRUE(arg.is_const);
    EXPECT_FALSE(arg.is_lvalue_reference);
    EXPECT_FALSE(arg.is_rvalue_reference);
    EXPECT_FALSE(arg.is_const_pointer);
  }
  {
    func_args arg = trait_args<int32_t*&>();
    EXPECT_FALSE(arg.is_const);
    EXPECT_TRUE(arg.is_lvalue_reference);
    EXPECT_FALSE(arg.is_rvalue_reference);
    EXPECT_FALSE(arg.is_const_pointer);
  }
  {
    func_args arg = trait_args<int32_t*>();
    EXPECT_FALSE(arg.is_const);
    EXPECT_FALSE(arg.is_lvalue_reference);
    EXPECT_FALSE(arg.is_rvalue_reference);
    EXPECT_FALSE(arg.is_const_pointer);
  }  
}

TEST(TypeNameTest, func_args) {
  bool default_result = func_args_to_test<int, int&>();
  EXPECT_TRUE(default_result);
  default_result = func_args_to_test<int, int&&>();
  EXPECT_TRUE(default_result);
  default_result = func_args_to_test<int, const int>();
  EXPECT_TRUE(default_result);
  default_result = func_args_to_test<int, const int&>();
  EXPECT_TRUE(default_result);
  default_result = func_args_to_test<double, const double&>();
  EXPECT_TRUE(default_result);
  default_result = func_args_to_test<const int&, const int&>();
  EXPECT_TRUE(default_result);
  default_result = func_args_to_test<const int&, const int>();
  EXPECT_TRUE(default_result);
  default_result = func_args_to_test<const int, int>();
  EXPECT_TRUE(default_result);
  default_result = func_args_to_test<const int&, int>();
  EXPECT_TRUE(default_result);
  default_result = func_args_to_test<const double&, double>();
  EXPECT_TRUE(default_result);
  default_result = func_args_to_test<int&, int&>();
  EXPECT_TRUE(default_result);
  default_result = func_args_to_test<int&, int&&>();
  EXPECT_TRUE(default_result);
  default_result = func_args_to_test<int&, const int&>();
  EXPECT_TRUE(default_result);
  default_result = func_args_to_test<int&&, const int&>();
  EXPECT_TRUE(default_result);
  default_result = func_args_to_test<int&&, int&&>();
  EXPECT_TRUE(default_result);
  default_result = func_args_to_test<int*, int*>();
  EXPECT_TRUE(default_result);
  default_result = func_args_to_test<int*, const int*>();
  EXPECT_TRUE(default_result);
  default_result = func_args_to_test<int*, const int* const>();
  EXPECT_TRUE(default_result);
  default_result = func_args_to_test<int*, int* const>();
  EXPECT_TRUE(default_result);
  default_result = func_args_to_test<const int*, const int* const>();
  EXPECT_TRUE(default_result);
  default_result = func_args_to_test<int* const, int*>();
  EXPECT_TRUE(default_result);
  default_result = func_args_to_test<int*&, int*>();
  EXPECT_TRUE(default_result);
  default_result = func_args_to_test<const int*&, const int*>();
  EXPECT_TRUE(default_result);
  default_result = func_args_to_test<const int*, const int*&>();
  EXPECT_TRUE(default_result);
  default_result = func_args_to_test<int*const&, int*>();
  EXPECT_TRUE(default_result);
  default_result = func_args_to_test<const int*const&, const int*>();
  EXPECT_TRUE(default_result);

  default_result = func_args_to_test<const int&, int&>();
  EXPECT_FALSE(default_result);
  default_result = func_args_to_test<const int&, int&&>();
  EXPECT_FALSE(default_result);
  default_result = func_args_to_test<const int&, int&&>();
  EXPECT_FALSE(default_result);
  default_result = func_args_to_test<const int*, int*>();
  EXPECT_FALSE(default_result);
  default_result = func_args_to_test<const int*, int* const>();
  EXPECT_FALSE(default_result);
  default_result = func_args_to_test<const int*&, int*>();
  EXPECT_FALSE(default_result);
  default_result = func_args_to_test<const int*const&, const int*&>();
  EXPECT_FALSE(default_result);  
}

} // namespace func_signature_detail

} // namespace reflection