#include "reflection.h"

#include "gtest/gtest.h"

struct TestObject : public reflection::meta_object {
  int i;
  std::string name;

  static reflection::meta_item_spec<TestObject, int, &TestObject::i> i_meta;
  const reflection::meta_info& get_meta_info() const override {
    static reflection::meta_info_spec<
        TestObject, reflection::meta_item_spec<TestObject, int, &TestObject::i>,
        reflection::meta_item_spec<TestObject, std::string, &TestObject::name>>
        info("TestObject", "int", "i", "std::string", "name");
    return info;
  }

  const reflection::serializer_base& get_serializer() const {
    static reflection::serializer<TestObject> inst;
    return inst;
  }
};

reflection::meta_item_spec<TestObject, int, &TestObject::i> TestObject::i_meta("int", "i");

struct Person : public reflection::meta_object {
  reflected(Person, (int, id, = 3), (std::string, name));
};
register_to_meta_manager(Person);

namespace interface {
struct Widget : public reflection::meta_object {
  reflected(interface::Widget, (int, width, = 0), (int, height));
};

register_to_meta_manager(Widget);

}; // namespace interface

// namespace test

namespace reflection {
namespace another {
struct widget : public meta_object {
  void general_func() const;
  reflected(reflection::another::widget, (int, id, = 233));
  reflected_func(reflection::another::widget, (general_func));
};
} // namespace another
} // namespace reflection

TEST(ReflectionTest, MetaItem) {
  TestObject inst;
  inst.i = 233;
  EXPECT_EQ(233, TestObject::i_meta.get(&inst));

  TestObject::i_meta.set(&inst, 234);
  EXPECT_EQ(234, inst.i);
}

TEST(ReflectionTest, MetaInfo) {
  TestObject inst;
  const auto& meta_info = inst.get_meta_info();
  EXPECT_TRUE(meta_info.has("i"));
  EXPECT_TRUE(meta_info.has("name"));

  EXPECT_EQ(2, meta_info.item_map().size());
  std::string move_v = "hello";
  inst.set<std::string>("name", std::move(move_v)).set<int>("i", 233);
  EXPECT_EQ(233, inst.i);
  EXPECT_EQ("hello", inst.name);
  EXPECT_EQ(233, meta_info.get_meta_item("i").get<int>(&inst));
  EXPECT_EQ("hello", meta_info.get_meta_item("name").get<std::string>(&inst));
}

TEST(ReflectionTest, PersonTest) {
  Person p;
  EXPECT_EQ(3, p.id);
  p.id = 233;
  p.name = "test";

  EXPECT_TRUE(p.has_meta("id"));
  EXPECT_TRUE(p.has_meta("name"));
  EXPECT_EQ(std::string("int"), p.get_meta_item("id").type());
  EXPECT_EQ(std::string("std::string"), p.get_meta_item("name").type());

  EXPECT_EQ(233, p.get<int>("id"));
  EXPECT_EQ("test", p.get<std::string>("name"));

  p.set<int>("id", 234).set<std::string>("name", "unknown");
  EXPECT_EQ(234, p.get<int>("id"));
  EXPECT_EQ("unknown", p.get<std::string>("name"));
}

TEST(ReflectionTest, CreateTest) {
  EXPECT_TRUE(reflection::meta_info_manager::get().has("Person"));
  std::unique_ptr<reflection::meta_object> p =
      reflection::meta_info_manager::get().create("Person");
  EXPECT_TRUE(p != nullptr);
  p->set("name", "Tony");
  EXPECT_EQ("Tony", p->get<std::string>("name"));
}

TEST(ReflectionTest, NamespaceTest) {
  const reflection::meta_info_manager& manager = reflection::meta_info_manager::get();
  EXPECT_TRUE(manager.has("interface::Widget"));
  std::unique_ptr<reflection::meta_object> p = manager.create("interface::Widget");
  EXPECT_TRUE(p != nullptr);
  EXPECT_EQ(0, p->get<int>("width"));
  p->set("width", 467);
  EXPECT_EQ(467, p->get<int>("width"));
}