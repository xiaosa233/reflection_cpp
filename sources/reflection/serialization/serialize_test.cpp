#include "gtest/gtest.h"

#include "reflection.h"
#include "reflection/utils.h"

namespace reflection {

namespace {

template <class T>
const char* check_size_meta(const T expected_size, const char* ptr) {
  T cp_size = *reinterpret_cast<const T*>(ptr);
  byte_swapper<sizeof(T)>::swap(&cp_size);
  EXPECT_EQ(expected_size, cp_size);
  return ptr + sizeof(T);
}

const char* check_item_name(const std::string_view& name, const char* ptr) {
  ptr = check_size_meta<uint16_t>(static_cast<uint16_t>(name.size()), ptr);
  EXPECT_EQ(name, std::string_view(ptr, name.size()));
  return ptr + name.size();
}

template <class T>
const char* check_item_value(const T& inst, const char* ptr) {
  ptr = check_size_meta<uint32_t>(sizeof(T), ptr);
  T cp_inst = *reinterpret_cast<const T*>(ptr);
  byte_swapper<sizeof(T)>::swap(&cp_inst);
  EXPECT_EQ(inst, cp_inst);
  return ptr + sizeof(T);
}

} // namespace

struct Person : public meta_object {
  reflected(Person, (int, id), (std::string, name), (double, height), (float, weight));
};

struct Widget : public meta_object {
  reflected(Widget, (int, size), (Person, person));
};

TEST(SerializerTest, serialize_to_string) {

  {
    // for general
    int a = 233;
    std::string out;
    expect_ok(general_serializer<int>::serialize_to_string(a, &out, nullptr));
    EXPECT_EQ("233", out);
    out.clear();
    format_context context;
    expect_ok(general_serializer<int>::serialize_to_string(a, &out, &context));
    EXPECT_EQ("233", out);
  }
  {
    // for vector
    std::vector<int> a = {1, 2, 3, 4, 5};
    std::string out;
    expect_ok(vector_serializer<std::vector<int>>::serialize_to_string(a, &out, nullptr));
    EXPECT_EQ("[1,2,3,4,5]", out);

    out.clear();
    format_context context;
    expect_ok(vector_serializer<std::vector<int>>::serialize_to_string(a, &out, &context));
    EXPECT_EQ("[1, 2, 3, 4, 5]", out);

    out.clear();
    context.trivial_value_new_line_size = 2;
    expect_ok(vector_serializer<std::vector<int>>::serialize_to_string(a, &out, &context));
    EXPECT_EQ("[\n    1, 2,\n    3, 4,\n    5\n]", out);
  }
  {
    // vector with vector
    std::vector<std::vector<int>> a = {{1, 2, 3}, {4, 5, 6}};
    std::string out;
    expect_ok(
        vector_serializer<std::vector<std::vector<int>>>::serialize_to_string(a, &out, nullptr));
    EXPECT_EQ("[[1,2,3],[4,5,6]]", out);

    out.clear();
    format_context context;
    expect_ok(
        vector_serializer<std::vector<std::vector<int>>>::serialize_to_string(a, &out, &context));
    EXPECT_EQ("[\n    [1, 2, 3],\n    [4, 5, 6]\n]", out);
  }
  {
    // for object
    Person p;
    p.id = 233;
    p.name = "jack";
    p.height = 2.33;
    p.weight = 1.f;
    std::string out;
    expect_ok(object_serializer::serialize_to_string(p, &out, nullptr));
    EXPECT_EQ("{id:233,name:\"jack\",height:2.3300000000000001,weight:1}", out);

    out.clear();
    format_context context;
    expect_ok(object_serializer::serialize_to_string(p, &out, &context));
    EXPECT_EQ(
        "{\n    id: 233,\n    name: \"jack\",\n    height: 2.3300000000000001,\n    weight: 1\n}",
        out);

    // object within object
    Widget w;
    w.size = 5;
    w.person = p;
    out.clear();
    expect_ok(object_serializer::serialize_to_string(w, &out, nullptr));
    EXPECT_EQ("{size:5,person:{id:233,name:\"jack\",height:2.3300000000000001,weight:1}}", out);

    out.clear();
    context.tab_space_count = 2;
    expect_ok(object_serializer::serialize_to_string(w, &out, &context));
    EXPECT_EQ("{\n  size: 5,\n  person: {\n    id: 233,\n    name: \"jack\",\n    height: "
              "2.3300000000000001,\n    weight: 1\n  }\n}",
              out);
  }
  {
    // vector with object
    std::vector<Person> a = {aggregate_initialize<Person>(1, "jack", 2.33, 1.f),
                             aggregate_initialize<Person>(2, "Mike", 3.33, 2.f)};
    std::string out;
    expect_ok(vector_serializer<decltype(a)>::serialize_to_string(a, &out, nullptr));
    EXPECT_EQ(
        "[{id:1,name:\"jack\",height:2.3300000000000001,weight:1},{id:2,name:\"Mike\",height:3."
        "3300000000000001,weight:2}]",
        out);

    out.clear();
    format_context context;
    context.tab_space_count = 2;
    expect_ok(vector_serializer<decltype(a)>::serialize_to_string(a, &out, &context));
    EXPECT_EQ("[\n  {\n    id: 1,\n    name: \"jack\",\n    height: 2.3300000000000001,\n    "
              "weight: 1\n  },\n  {\n    id: 2,\n    name: \"Mike\",\n    height: 3."
              "3300000000000001,\n    weight: 2\n  }\n]",
              out);
  }
}

TEST(SerializerTest, serialize_from_string) {
  {
    int a = 0;
    expect_error(general_serializer<int>::serialize_from_string(" asdf233 ", &a));
    expect_ok(general_serializer<int>::serialize_from_string(" 233 ", &a));
    EXPECT_EQ(233, a);
  }
  {
    std::vector<int> a;
    expect_ok(vector_serializer<std::vector<int>>::serialize_from_string("[1,2,3]", &a));
    EXPECT_EQ(3, a.size());
    EXPECT_EQ(1, a[0]);
    EXPECT_EQ(2, a[1]);
    EXPECT_EQ(3, a[2]);
  }
  {
    std::vector<std::vector<int>> a;
    expect_ok(vector_serializer<decltype(a)>::serialize_from_string("[[1,2,3],[4,5,6,],]", &a));
    EXPECT_EQ(2, a.size());
    EXPECT_EQ(1, a[0][0]);
    EXPECT_EQ(2, a[0][1]);
    EXPECT_EQ(3, a[0][2]);
    EXPECT_EQ(4, a[1][0]);
    EXPECT_EQ(5, a[1][1]);
    EXPECT_EQ(6, a[1][2]);
  }
  {
    Person p;
    expect_ok(object_serializer::serialize_from_string(
        "{id:233,name:\"jack\",height:2.3300000000000001,weight:1}", &p));
    EXPECT_EQ(233, p.id);
    EXPECT_EQ("jack", p.name);
    EXPECT_EQ(2.33, p.height);
    EXPECT_EQ(1.f, p.weight);
  }
  {
    Widget w;
    expect_ok(object_serializer::serialize_from_string(
        "{size:5,person:{id:233,name:\"jack\",height:2.3300000000000001,weight:1}}", &w));
    EXPECT_EQ(5, w.size);
    EXPECT_EQ(233, w.person.id);
    EXPECT_EQ("jack", w.person.name);
    EXPECT_EQ(2.33, w.person.height);
    EXPECT_EQ(1.f, w.person.weight);
  }
}

TEST(SerializerTest, serialize_with_bytes) {
  {
    int32_t a = 0x01020304;
    std::vector<char> out;
    expect_ok(general_serializer<int32_t>::serialize_to_bytes(a, endian::big, &out));
    EXPECT_EQ(4, out.size());
    EXPECT_EQ(0x01, out[0]);
    EXPECT_EQ(0x02, out[1]);
    EXPECT_EQ(0x03, out[2]);
    EXPECT_EQ(0x04, out[3]);

    int32_t to_a = 0;
    expect_ok(general_serializer<int32_t>::serialize_from_bytes(out.data(), out.data() + out.size(),
                                                                endian::big, &to_a));
    EXPECT_EQ(0x01020304, to_a);
  }
  {
    std::vector<int32_t> a = {0x01020304};
    std::vector<char> out;
    expect_ok(vector_serializer<std::vector<int32_t>>::serialize_to_bytes(a, endian::big, &out));
    EXPECT_EQ(0x01, out[0]);
    EXPECT_EQ(0x02, out[1]);
    EXPECT_EQ(0x03, out[2]);
    EXPECT_EQ(0x04, out[3]);

    std::vector<int32_t> to_a;
    expect_ok(vector_serializer<decltype(to_a)>::serialize_from_bytes(
        out.data(), out.data() + out.size(), endian::big, &to_a));
    EXPECT_EQ(to_a.size(), a.size());
    EXPECT_EQ(0x01020304, to_a[0]);

    // vector with vectors
    std::vector<decltype(a)> aa = {a};
    out.clear();
    expect_ok(vector_serializer<decltype(aa)>::serialize_to_bytes(aa, endian::big, &out));
    EXPECT_EQ(8, out.size());
    EXPECT_EQ(0, out[0]);
    EXPECT_EQ(0, out[1]);
    EXPECT_EQ(0, out[2]);
    EXPECT_EQ(4, out[3]);
    EXPECT_EQ(0x01, out[4]);
    EXPECT_EQ(0x02, out[5]);
    EXPECT_EQ(0x03, out[6]);
    EXPECT_EQ(0x04, out[7]);

    std::vector<decltype(a)> to_aa;
    expect_ok(vector_serializer<decltype(to_aa)>::serialize_from_bytes(
        out.data(), out.data() + out.size(), endian::big, &to_aa));
    EXPECT_EQ(to_aa.size(), aa.size());
    EXPECT_EQ(a.size(), to_aa.front().size());
    EXPECT_EQ(0x01020304, to_aa[0][0]);
  }
  {
    Person p = aggregate_initialize<Person>(233, "jack", 2.33, 1.f);
    std::vector<char> out;
    expect_ok(object_serializer::serialize_to_bytes(p, get_opposite_endian(endian::native), &out));
    EXPECT_EQ(62, out.size());
    const char* ptr = out.data();
    ptr = check_item_name("id", ptr);
    ptr = check_item_value(233, ptr);

    ptr = check_item_name("name", ptr);

    ptr = check_size_meta<uint32_t>(4, ptr);
    EXPECT_EQ("jack", std::string_view(ptr, 4));
    ptr += 4;

    ptr = check_item_name("height", ptr);
    ptr = check_item_value(2.33, ptr);

    ptr = check_item_name("weight", ptr);
    ptr = check_item_value(1.f, ptr);

    Person to_p;
    expect_ok(object_serializer::serialize_from_bytes(out.data(), out.data() + out.size(),
                                                      endian::big, &to_p));
    EXPECT_EQ(p.name, to_p.name);
    EXPECT_EQ(p.id, to_p.id);
    EXPECT_EQ(p.height, to_p.height);
    EXPECT_EQ(p.weight, to_p.weight);

    // object with object
    Widget w = aggregate_initialize<Widget>(5, p);
    std::vector<char> w_out;
    expect_ok(
        object_serializer::serialize_to_bytes(w, get_opposite_endian(endian::native), &w_out));
    const char* w_ptr = w_out.data();
    w_ptr = check_item_name("size", w_ptr);
    w_ptr = check_item_value(w.size, w_ptr);
    w_ptr = check_item_name("person", w_ptr);
    w_ptr = check_size_meta<uint32_t>(static_cast<uint32_t>(out.size()), w_ptr);
    EXPECT_EQ(out.size(), w_out.size() - (w_ptr - w_out.data()));
    EXPECT_TRUE(memcmp(out.data(), w_ptr, out.size()) == 0);

    Widget to_w;
    expect_ok(object_serializer::serialize_from_bytes(w_out.data(), w_out.data() + w_out.size(),
                                                      endian::big, &to_w));
    EXPECT_EQ(w.size, to_w.size);
    EXPECT_EQ(w.person.name, to_w.person.name);
    EXPECT_EQ(w.person.id, to_w.person.id);
    EXPECT_EQ(w.person.height, to_w.person.height);
    EXPECT_EQ(w.person.weight, to_w.person.weight);
  }
}

TEST(SerializeTest, ToText) {
  Person person;
  {
    EXPECT_EQ("Person", person.class_name());
    EXPECT_TRUE(person.has_meta("id"));
    EXPECT_TRUE(person.has_meta("name"));
    EXPECT_TRUE(person.has_meta("height"));
    EXPECT_TRUE(person.has_meta("weight"));
    // serialize from string
    person.member_from_string("id", "233");
    person.member_from_string("name", "jack");
    person.member_from_string("height", "1.73456789");
    person.member_from_string("weight", "50.12");

    person.id = 233;
    person.name = "jack";
    person.height = 1.73456789;
    person.weight = 50.12f;
    std::string result;
    EXPECT_TRUE(person.serialize_to_string(&result));
    EXPECT_EQ("{id:233,name:\"jack\",height:1.7345678899999999,weight:50.1199989}", result);
  }

  {
    // Serialize with second meta_object
    Widget widget;
    widget.person = person;
    widget.size = 233;
    std::string result;
    EXPECT_TRUE(widget.serialize_to_string(&result));
    EXPECT_EQ(
        "{size:233,person:{id:233,name:\"jack\",height:1.7345678899999999,weight:50.1199989}}",
        result);
  }
}

TEST(SerializeTest, optional_to_text) {
  std::optional<Person> value;
  std::string output;
  format_context context;
  expect_ok(optional_serializer<decltype(value)>::serialize_to_string(value, &output, &context));
  EXPECT_TRUE(output.empty());

  value = aggregate_initialize<Person>(233, "jack", 2.33, 1.f);
  expect_ok(optional_serializer<decltype(value)>::serialize_to_string(value, &output, &context));
  EXPECT_EQ("{\n    id: 233,\n    name: \"jack\",\n    height: 2.3300000000000001,\n    weight: 1\n}", output);

  value.reset();
  expect_ok(optional_serializer<decltype(value)>::serialize_from_string(output, &value));
  EXPECT_TRUE(value);
  EXPECT_EQ(233, value->id);
  EXPECT_EQ("jack", value->name);
  EXPECT_EQ(2.33, value->height);
  EXPECT_EQ(1.f, value->weight);

  std::vector<char> buffer;
  expect_ok(optional_serializer<decltype(value)>::serialize_to_bytes({}, endian::native, &buffer));
  EXPECT_TRUE(buffer.empty());
  expect_ok(
      optional_serializer<decltype(value)>::serialize_to_bytes(value, endian::native, &buffer));

  std::optional<Person> expect_p;
  expect_ok(optional_serializer<decltype(value)>::serialize_from_bytes(
      buffer.data(), buffer.data() + buffer.size(), endian::native, &expect_p));
  EXPECT_TRUE(expect_p);
  EXPECT_EQ(233, expect_p->id);
  EXPECT_EQ("jack", expect_p->name);
  EXPECT_EQ(2.33, expect_p->height);
  EXPECT_EQ(1.f, expect_p->weight);
}

} // namespace reflection
