#include "utils.h"

#include <filesystem>

#include "gtest/gtest.h"

#include "reflection.h"

namespace reflection {

struct Person : public meta_object {
  reflected(Person, (int, id), (std::string, name), (double, height), (float, weight, = 1.f));
};

struct Widget : public meta_object {
  reflected(Widget, (int, size), (Person, person));
};

TEST(UtilsTest, aggregate_initialize) {
  {
    Person p = aggregate_initialize<Person>(233, "jack", 2.33);
    EXPECT_EQ(233, p.id);
    EXPECT_EQ("jack", p.name);
    EXPECT_EQ(2.33, p.height);
    // default for weight
    EXPECT_EQ(1.f, p.weight);
    Widget w = aggregate_initialize<Widget>(5, p);
    EXPECT_EQ(5, w.size);
    EXPECT_EQ(233, w.person.id);
    EXPECT_EQ("jack", w.person.name);
    EXPECT_EQ(2.33, w.person.height);
    EXPECT_EQ(1.f, w.person.weight);
  }
}

TEST(UtilsTest, pair_initialize) {
  {
    Person p = pair_initialize<Person>(std::make_pair("id", 233), std::make_pair("height", 2.33),
                                       std::make_pair("name", "jack"));
    EXPECT_EQ(233, p.id);
    EXPECT_EQ("jack", p.name);
    EXPECT_EQ(2.33, p.height);
    // default for weight
    EXPECT_EQ(1.f, p.weight);

    Widget w =
        pair_initialize<Widget>(std::make_pair("size", 5), std::make_pair("person", std::move(p)));
    EXPECT_EQ(5, w.size);
    EXPECT_EQ(233, w.person.id);
    EXPECT_EQ("jack", w.person.name);
    EXPECT_EQ(2.33, w.person.height);
    EXPECT_EQ(1.f, w.person.weight);
  }
}

TEST(UtilsTest, serialize_with_file) {
  // TODO fixed ut
  Person p = pair_initialize<Person>(std::make_pair("id", 233), std::make_pair("height", 2.33),
                                     std::make_pair("name", "jack"));

  std::filesystem::path tmp_path = std::filesystem::temp_directory_path();
  const std::string text_file_path = (tmp_path / "reflection_ut" / "p.txt").string();
  expect_ok(serialize_to_text_file(p, text_file_path));
  Person string_result_p;
  expect_ok(serialize_from_text_file(text_file_path, &string_result_p));
  EXPECT_EQ(p.id, string_result_p.id);
  EXPECT_EQ(p.height, string_result_p.height);
  EXPECT_EQ(p.weight, string_result_p.weight);

  // remove tmp file
  EXPECT_TRUE(std::filesystem::remove(text_file_path));

  const std::string binary_file_path = (tmp_path / "reflection_ut" / "p.bin").string();
  {
    expect_ok(serialize_to_binary_file(p, binary_file_path, endian::big));

    Person binary_result_p;
    expect_ok(serialize_from_binary_file(binary_file_path, &binary_result_p));
    EXPECT_EQ(p.id, binary_result_p.id);
    EXPECT_EQ(p.height, binary_result_p.height);
    EXPECT_EQ(p.name, binary_result_p.name);
    EXPECT_EQ(p.weight, binary_result_p.weight);
  }

  {
    expect_ok(serialize_to_binary_file(p, binary_file_path, endian::little));

    Person binary_result_p;
    expect_ok(serialize_from_binary_file(binary_file_path, &binary_result_p));
    EXPECT_EQ(p.id, binary_result_p.id);
    EXPECT_EQ(p.height, binary_result_p.height);
    EXPECT_EQ(p.name, binary_result_p.name);
    EXPECT_EQ(p.weight, binary_result_p.weight);
  }

  EXPECT_TRUE(std::filesystem::remove(binary_file_path));
}

} // namespace reflection
