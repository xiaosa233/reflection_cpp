#include "gtest/gtest.h"

#include "func/string_callable.h"

namespace reflection {

struct widget {
  int general_func(int i) { return i; }

  int empty_func() const { return value; }

  static int static_func(int i) { return i; }

  std::vector<int> _2_func(int i, double j) const { return {i, static_cast<int>(j)}; }

  int value = 233;
};

TEST(StringCallableTest, string_callable) {
  widget w;
  EXPECT_EQ("233", string_callable<1>::call(
                       create_member_function_wrapper(&widget::general_func, &w), {"233"}));
  EXPECT_EQ("233",
            string_callable<0>::call(create_member_function_wrapper(&widget::empty_func, &w), {}));
  EXPECT_EQ("2", string_callable<1>::call(
                     create_member_function_wrapper(&widget::static_func, nullptr), {"2"}));

  EXPECT_EQ("[1,2]", string_callable<2>::call(create_member_function_wrapper(&widget::_2_func, &w),
                                              {"1", "2.33333"}));
}

} // namespace reflection
