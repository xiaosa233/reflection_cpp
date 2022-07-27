#include "status.h"

#include "gtest/gtest.h"

namespace reflection {

TEST(StatusTest, Constructor) {
  {
    status ok(0);
    EXPECT_EQ(ok, status::ok());
    EXPECT_TRUE(ok);
  }
  {
    status error(-1, "some error message");
    EXPECT_EQ(status::error(), error);
    EXPECT_FALSE(error);
  }
}

} // namespace reflection
