#include "meta/meta_base_classes.h"

#include "gtest/gtest.h"

#include "reflection.h"
#include "utils.h"

namespace reflection {

struct Person : public meta_object {
  reflected(Person, (int, id), (std::string, name), (double, height), (float, weight));
};

struct Widget : public meta_object {

  int general_func(int i, int j, int k) { return i + j + k; }
  int const_func(int i, int j, int k) const { return i + j + k; }

  reflected(Widget, (int, size), (Person, person), (std::vector<int>, ints),
            (std::optional<double>, optional_true), (std::optional<double>, optional_false),
            (std::vector<int>, empty_value),
            (std::vector<std::optional<int>>, vector_with_optionals));

  reflected_func(Widget, (general_func));
};

struct AllCase : public meta_object {
  reflected(AllCase, (int, general_int), (double, general_double), (Widget, general_object),
            (std::vector<int>, vec_int), (std::vector<Widget>, vec_object),
            (std::optional<int>, optional_false_int),
            (std::optional<Widget>, optional_false_object), (std::optional<int>, optional_true_int),
            (std::optional<Widget>, optional_true_object),
            (std::optional<std::vector<int>>, optional_empty_vec),
            (std::optional<std::vector<Widget>>, optional_vec_object),
            (std::vector<std::optional<int>>, vec_with_options));

  bool operator==(const AllCase& other) const { return equal(*this, other); }

  friend std::iostream& operator<<(std::iostream& stream, const AllCase& rhs) {
    std::string result;
    if (rhs.serialize_to_string(&result)) {
      stream << result;
    }
    return stream;
  }

private:
  bool equal(const meta_object& lhs, const meta_object& rhs) const {
    const meta_info& meta_info_v = lhs.get_meta_info();
    if (&meta_info_v != &rhs.get_meta_info()) {
      return false;
    }
    for (const auto& item : meta_info_v.item_vec()) {
      std::string first, second;
      return_if_error(lhs.member_to_string(item->name(), &first));
      return_if_error(rhs.member_to_string(item->name(), &second));
      if (first != second) {
        return false;
      }
    }
    return true;
  }
};

struct EmptyObject : public meta_object {};

class PrivateTestObject : public meta_object {

  void private_func() { ++id;}

  reflected(PrivateTestObject, (int, id, = 233));
  reflected_func(PrivateTestObject, (private_func));
};

TEST(MetaBaseClassesTest, PrivateTest) {
  PrivateTestObject object;
  EXPECT_EQ(233, object.get<int>("id"));
  expect_ok(object.invoke("private_func"));
  EXPECT_EQ(234, object.get<int>("id"));
}

TEST(MetaBaseClassesTest, SerializeTest) {
  // serialize with string
  Person p = aggregate_initialize<Person>(233, "jack", 2.33, 1.f);
  std::string out;
  expect_ok(p.member_to_string("id", &out));
  EXPECT_EQ("233", out);

  out.clear();
  expect_ok(p.serialize_to_string(&out));
  EXPECT_EQ("{id:233,name:\"jack\",height:2.3300000000000001,weight:1}", out);

  // serialize from string
  expect_ok(p.member_from_string("id", "234"));
  EXPECT_EQ(234, p.id);
  expect_ok(p.serialize_from_string(out));

  EXPECT_EQ(233, p.id);
  EXPECT_EQ("jack", p.name);
  EXPECT_EQ(2.33, p.height);
  EXPECT_EQ(1.f, p.weight);

  // object with object
  std::string w_out;
  Widget w =
      aggregate_initialize<Widget>(5, p, std::vector<int>{1, 2, 3}, std::optional<double>{0.233});
  expect_ok(w.member_to_string("person", &w_out));
  EXPECT_TRUE(w.optional_true);
  EXPECT_FALSE(w.optional_false);
  EXPECT_EQ(w_out, out);
  w.vector_with_optionals.resize(5);
  w.vector_with_optionals[1] = 2;

  w_out.clear();
  expect_ok(w.serialize_to_string(&w_out));
  EXPECT_EQ("{size:5,person:{id:233,name:\"jack\",height:2.3300000000000001,weight:1},ints:[1,2,3],"
            "optional_true:0.23300000000000001,vector_with_optionals:[2]}",
            w_out);

  Widget to_w;
  expect_ok(to_w.serialize_from_string(w_out));
  EXPECT_EQ(w.size, to_w.size);
  EXPECT_EQ(w.person.id, to_w.person.id);
  EXPECT_EQ(w.person.name, to_w.person.name);
  EXPECT_EQ(w.person.height, to_w.person.height);
  EXPECT_EQ(w.person.weight, to_w.person.weight);
  EXPECT_EQ(w.ints.size(), to_w.ints.size());
  EXPECT_EQ(3, w.ints.size());
  EXPECT_EQ(w.ints[0], to_w.ints[0]);
  EXPECT_EQ(w.ints[1], to_w.ints[1]);
  EXPECT_EQ(w.ints[2], to_w.ints[2]);
  EXPECT_TRUE(to_w.optional_true);
  EXPECT_EQ(0.233, to_w.optional_true.value());
  EXPECT_FALSE(to_w.optional_false);
  EXPECT_EQ(0, to_w.empty_value.size());
  EXPECT_EQ(1, to_w.vector_with_optionals.size());
  EXPECT_EQ(2, to_w.vector_with_optionals[0].value());

  // Serialize to bytes
  std::vector<char> bytes;
  expect_ok(w.serialize_to_bytes(&bytes));
  to_w = Widget();
  expect_ok(to_w.serialize_from_bytes(bytes.data(), bytes.data() + bytes.size()));
  EXPECT_EQ(w.size, to_w.size);
  EXPECT_EQ(w.person.id, to_w.person.id);
  EXPECT_EQ(w.person.name, to_w.person.name);
  EXPECT_EQ(w.person.height, to_w.person.height);
  EXPECT_EQ(w.person.weight, to_w.person.weight);
  EXPECT_EQ(w.ints.size(), to_w.ints.size());
  EXPECT_EQ(3, w.ints.size());
  EXPECT_EQ(w.ints[0], to_w.ints[0]);
  EXPECT_EQ(w.ints[1], to_w.ints[1]);
  EXPECT_EQ(w.ints[2], to_w.ints[2]);
  EXPECT_TRUE(to_w.optional_true);
  EXPECT_EQ(0.233, to_w.optional_true.value());
  EXPECT_FALSE(to_w.optional_false);
  EXPECT_EQ(0, to_w.empty_value.size());
  EXPECT_EQ(1, to_w.vector_with_optionals.size());
  EXPECT_EQ(2, to_w.vector_with_optionals[0].value());

  {
    AllCase all_case;
    all_case.general_int = 233;
    all_case.general_double = 0.23;
    all_case.general_object = w;
    all_case.vec_int = {1, 2, 3};
    all_case.vec_object = {w, w, w};
    all_case.optional_true_int = 233;
    all_case.optional_true_object = w;
    all_case.optional_empty_vec = std::vector<int>();
    all_case.optional_vec_object = all_case.vec_object;
    all_case.vec_with_options.resize(5);
    all_case.vec_with_options[0] = 233;

    std::string out;
    expect_ok(all_case.serialize_to_string(&out));
    AllCase to_all_case;
    expect_ok(to_all_case.serialize_from_string(out));
    EXPECT_TRUE(all_case == to_all_case)
        << object_to_string(all_case) << "\n-------------- vs --------------\n"
        << object_to_string(to_all_case);

    out.clear();
    format_context context;
    expect_ok(all_case.serialize_to_string(&out, &context));
    to_all_case = AllCase();
    expect_ok(to_all_case.serialize_from_string(out));
    EXPECT_TRUE(all_case == to_all_case)
        << object_to_string(all_case) << "\n-------------- vs --------------\n"
        << object_to_string(to_all_case);

    std::vector<char> bytes;
    expect_ok(all_case.serialize_to_bytes(&bytes));
    to_all_case = AllCase();
    expect_ok(to_all_case.serialize_from_bytes(bytes.data(), bytes.data() + bytes.size()));
    EXPECT_TRUE(all_case == to_all_case)
        << object_to_string(all_case) << "\n-------------- vs --------------\n"
        << object_to_string(to_all_case);
  }
}

} // namespace reflection
