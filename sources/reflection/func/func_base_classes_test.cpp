#include "func/func_base_classes.h"

#include "gtest/gtest.h"

#include "func/func_info_spec.h"
#include "func/func_item_spec.h"
#include "reflection.h"

namespace reflection {

struct Base {
  virtual int fun() { return 1; }
};

struct B : public Base {
  virtual int fun() override { return 2; }
};
struct C : public Base {
  virtual int fun() override { return 3; }
};
struct D : public B, public C {
  virtual int fun() override { return 4; }
};

struct VirtualB : virtual public Base {
  virtual int fun() override { return 2; }
};
struct VirtualC : virtual public Base {
  virtual int fun() override { return 3; }
};
struct VirtualD : public VirtualB, public VirtualC {
  virtual int fun() override { return 4; }
};

class ReflectionFuncObject : public meta_object {
public:
  int general_func(int i, double j) { return i + static_cast<int>(j); }

  int default_param_func(int i, double j = 0.0) { return i + static_cast<int>(j); }

  int overload_func(int i) { return i; }

  double overload_func(double i) { return i; }

  int const_func(int i) const { return i; }

  virtual int virtual_func(int i) { return i; }

  static int static_general_func(ReflectionFuncObject* object) { return object->i; }

  int left_reference_func(const ReflectionFuncObject& object) { return object.i; }

  int right_reference_func(ReflectionFuncObject&& object) { return object.i; }

  int volatile_func(volatile int i) { return i; }

  bool operator==(const ReflectionFuncObject& obj) const { return i == obj.i; }

  std::string string_call(const std::string& input) { return input; }

  int virtual_public_func(Base* base) { return base->fun(); }

  template <class T>
  int template_func(const T& j) {
    return i + static_cast<int>(j);
  }

  void void_general_func(int i, double j) {}

  // Not support friend function.
  reflected(ReflectionFuncObject, (int, a));
  reflected_func(ReflectionFuncObject, (general_func), (default_param_func), (const_func),
                 (virtual_func), (static_general_func), (left_reference_func),
                 (right_reference_func), (volatile_func), (operator==), (template_func<int>),
                 (void_general_func), (string_call), (virtual_public_func));

private:
  int i = 233;
};

// Check callable is legal.
static_assert(is_arg_serializable<ReflectionFuncObject>::value,
              "ReflectionFuncObject should be true");
static_assert(!is_arg_serializable<ReflectionFuncObject*>::value,
              "ReflectionFuncObject*  should be false");
static_assert(!is_arg_serializable<ReflectionFuncObject&>::value,
              "ReflectionFuncObject& should be false");
static_assert(is_arg_serializable<ReflectionFuncObject&&>::value,
              "ReflectionFuncObject&& should be true");
static_assert(is_arg_serializable<const ReflectionFuncObject&>::value,
              "const ReflectionFuncObject& should be true");

struct DerivedObject : public ReflectionFuncObject {

  int virtual_func(int i) override { return i + 1; } // namespace reflection
};

#define declare_func_item(value, ClassT, func_name)                                                \
  auto value = func_item_spec<ClassT, decltype(&ClassT::func_name)>(#func_name, &ClassT::func_name)

#define declare_overload_func_item(value, ClassT, types, func_name)                                \
  auto value = func_item_spec<ClassT, types>(#func_name, &ClassT::func_name);

declare_func_item(general_item, ReflectionFuncObject, general_func);
declare_func_item(default_param_item, ReflectionFuncObject, default_param_func);
declare_overload_func_item(overload_item, ReflectionFuncObject, int (ReflectionFuncObject::*)(int),
                           overload_func);
declare_func_item(const_item, ReflectionFuncObject, const_func);
declare_func_item(virtual_item, ReflectionFuncObject, virtual_func);
declare_func_item(static_general_item, ReflectionFuncObject, static_general_func);
declare_func_item(left_reference_item, ReflectionFuncObject, left_reference_func);
declare_func_item(right_reference_item, ReflectionFuncObject, right_reference_func);
declare_func_item(volatile_item, ReflectionFuncObject, volatile_func);
declare_func_item(operator_item, ReflectionFuncObject, operator==);
declare_func_item(template_int_item, ReflectionFuncObject, template_func<int>);
declare_func_item(template_double_item, ReflectionFuncObject, template_func<double>);

#define expect_context_status_ok(value, status)                                                    \
  do {                                                                                             \
    const auto tmp_status = status;                                                                \
    expect_ok(tmp_status);                                                                         \
    EXPECT_EQ(value, tmp_status.get());                                                            \
  } while (false)

TEST(FuncItemTest, func_item) {
  ReflectionFuncObject object;
  {
    EXPECT_EQ(std::string_view("general_func"), general_item.func_name());
    EXPECT_EQ("signed_integral_4(signed_integral_4,floating_point_8)",
              general_item.get_signature().to_string());
    const ReflectionFuncObject* const_ptr = &object;
    expect_context_status_ok(3, general_item.invoke(&object, 2, 1.0));
    const func_item* item = &object.get_func_info().get_func_item("general_func");
    expect_context_status_ok(3, item->invoke<int>(&object, 2, 1.0));
    expect_context_status_ok(3, item->invoke<int>(const_ptr, 2, 1.0));

    string_status status = item->invoke_by_string({"2", "1.0"}, &object);
    expect_context_status_ok("3", status);

    EXPECT_FALSE(item->is_static());
    EXPECT_FALSE(item->is_const());
    EXPECT_TRUE(item->is_serializable());
    EXPECT_EQ(2, item->args_size());

    // ERROR! can not change the type of the parameters.
    // EXPECT_EQ(5, item->invoke<int>(&object, 2, 3.f));

    expect_context_status_ok(3, object.invoke<int>(item->func_name(),2, 1.0));
    expect_context_status_ok("3", object.invoke_by_string(item->func_name(), {"2", "1.0"}));
  }
  {
    EXPECT_EQ(std::string_view("default_param_func"), default_param_item.func_name());
    expect_context_status_ok(3, default_param_item.invoke(&object, 2, 1.0));

    // ERROR! Can not deal with the default paramters
    // EXPECT_EQ(2, default_param_item.invoke(&object, 2));

    const func_item* item = &object.get_func_info().get_func_item("default_param_func");
    expect_context_status_ok(3, item->invoke<int>(&object, 2, 1.0));
    expect_context_status_ok("3", item->invoke_by_string({"2", "1.0"}, &object));

    EXPECT_FALSE(item->is_static());
    EXPECT_FALSE(item->is_const());
    EXPECT_TRUE(item->is_serializable());
    EXPECT_EQ(2, item->args_size());

    expect_context_status_ok(3, object.invoke<int>(item->func_name(), 2, 1.0));
    expect_context_status_ok("3", object.invoke_by_string(item->func_name(), {"2", "1.0"}));
  }
  {
    EXPECT_EQ(std::string_view("overload_func"), overload_item.func_name());
    expect_context_status_ok(2, overload_item.invoke(&object, 2));

    func_item* item = &overload_item;
    expect_context_status_ok(2, item->invoke<int>(&object, 2));
    string_status status = item->invoke_by_string({"2"}, &object);
    expect_ok(status);
    EXPECT_EQ("2", status.get());

    EXPECT_FALSE(item->is_static());
    EXPECT_FALSE(item->is_const());
    EXPECT_TRUE(item->is_serializable());
    EXPECT_EQ(1, item->args_size());
  }
  {
    EXPECT_EQ(std::string_view("const_func"), const_item.func_name());
    expect_context_status_ok(2, const_item.invoke(&object, 2));

    const func_item* item = &object.get_func_info().get_func_item("const_func");
    expect_context_status_ok(2, item->invoke<int>(&object, 2));
    string_status status = item->invoke_by_string({"2"}, &object);
    expect_ok(status);
    EXPECT_EQ("2", status.get());

    EXPECT_FALSE(item->is_static());
    EXPECT_TRUE(item->is_const());
    EXPECT_TRUE(item->is_serializable());
    EXPECT_EQ(1, item->args_size());
    expect_context_status_ok(2, object.invoke<int>(item->func_name(), 2));
    expect_context_status_ok("2", object.invoke_by_string(item->func_name(), {"2"}));
  }
  {
    EXPECT_EQ(std::string_view("virtual_func"), virtual_item.func_name());
    expect_context_status_ok(2, virtual_item.invoke(&object, 2));

    const func_item* item = &object.get_func_info().get_func_item("virtual_func");
    expect_context_status_ok(2, item->invoke<int>(&object, 2));

    DerivedObject derived_obj;
    expect_context_status_ok(3, virtual_item.invoke(&derived_obj, 2));
    expect_context_status_ok(3, item->invoke<int>(&derived_obj, 2));
    string_status status = item->invoke_by_string({"2"}, &object);
    expect_ok(status);
    EXPECT_EQ("2", status.get());

    status = item->invoke_by_string({"2"}, &derived_obj);
    expect_ok(status);
    EXPECT_EQ("3", status.get());

    EXPECT_FALSE(item->is_static());
    EXPECT_FALSE(item->is_const());
    EXPECT_TRUE(item->is_serializable());
    EXPECT_EQ(1, item->args_size());

    expect_context_status_ok(2, object.invoke<int>(item->func_name(), 2));
    expect_context_status_ok("2", object.invoke_by_string(item->func_name(), {"2"}));
  }
  {
    EXPECT_EQ(std::string_view("static_general_func"), static_general_item.func_name());
    expect_context_status_ok(233, static_general_item.invoke(&object));

    const func_item* item = &object.get_func_info().get_func_item("static_general_func");
    expect_context_status_ok(233, item->invoke<int>(&object));
    expect_error(item->invoke_by_string({"{}"}));

    EXPECT_TRUE(item->is_static());
    EXPECT_FALSE(item->is_const());
    EXPECT_FALSE(item->is_serializable());
    EXPECT_EQ(1, item->args_size());

    expect_context_status_ok(233, object.invoke<int>(item->func_name(), &object));
    expect_error(object.invoke_by_string(item->func_name(), {"{}"}));
  }
  {
    EXPECT_EQ(std::string_view("left_reference_func"), left_reference_item.func_name());
    expect_context_status_ok(233, left_reference_item.invoke(&object, object));

    const func_item* item = &object.get_func_info().get_func_item("left_reference_func");
    expect_context_status_ok(233, item->invoke<int>(&object, object));
    expect_context_status_ok(233, left_reference_item.invoke(&object, ReflectionFuncObject()));
    expect_context_status_ok(233, item->invoke<int>(&object, ReflectionFuncObject()));
    string_status status = item->invoke_by_string({"{}"}, &object);
    expect_ok(status);
    EXPECT_EQ("233", status.get());

    EXPECT_FALSE(item->is_static());
    EXPECT_FALSE(item->is_const());
    EXPECT_TRUE(item->is_serializable());
    EXPECT_EQ(1, item->args_size());
    expect_context_status_ok(233, object.invoke<int>(item->func_name(), object));
    expect_context_status_ok("233", object.invoke_by_string(item->func_name(), {"{}"}));
  }
  {
    EXPECT_EQ(std::string_view("right_reference_func"), right_reference_item.func_name());
    expect_context_status_ok(233, right_reference_item.invoke(&object, std::move(object)));

    const func_item* item = &object.get_func_info().get_func_item("right_reference_func");
    expect_context_status_ok(233, item->invoke<int>(&object, std::move(object)));
    expect_context_status_ok(233, item->invoke<int>(&object, object)); // OK, but not legal
    expect_context_status_ok(233, right_reference_item.invoke(&object, ReflectionFuncObject()));
    expect_context_status_ok(233, item->invoke<int>(&object, ReflectionFuncObject()));
    string_status status = item->invoke_by_string({"{}"}, &object);
    expect_ok(status);
    EXPECT_EQ("233", status.get());

    EXPECT_FALSE(item->is_static());
    EXPECT_FALSE(item->is_const());
    EXPECT_TRUE(item->is_serializable());
    EXPECT_EQ(1, item->args_size());
    expect_context_status_ok(233, object.invoke<int>(item->func_name(), object));
    expect_context_status_ok(233, object.invoke<int>(item->func_name(), std::move(object)));
    expect_context_status_ok("233", object.invoke_by_string(item->func_name(), {"{}"}));
  }
  {
    EXPECT_EQ(std::string_view("volatile_func"), volatile_item.func_name());
    expect_context_status_ok(233, volatile_item.invoke(&object, 233));

    const func_item* item = &object.get_func_info().get_func_item("volatile_func");
    expect_context_status_ok(233, item->invoke<int>(&object, 233));
    volatile int i = 233;
    expect_context_status_ok(233, volatile_item.invoke(&object, i));
    expect_context_status_ok(233, item->invoke<int>(&object, i));
    string_status status = item->invoke_by_string({"233"}, &object);
    expect_ok(status);
    EXPECT_EQ("233", status.get());

    EXPECT_FALSE(item->is_static());
    EXPECT_FALSE(item->is_const());
    EXPECT_TRUE(item->is_serializable());
    EXPECT_EQ(1, item->args_size());
    expect_context_status_ok(233, object.invoke<int>(item->func_name(), i));
    expect_context_status_ok("233", object.invoke_by_string(item->func_name(), {"233"}));
  }
  {
    EXPECT_EQ(std::string_view("operator=="), operator_item.func_name());
    expect_context_status_ok(true, operator_item.invoke(&object, ReflectionFuncObject()));
    expect_context_status_ok(true, operator_item.invoke(&object, object));

    const func_item* item = &object.get_func_info().get_func_item("operator==");
    expect_context_status_ok(true, item->invoke<bool>(&object, ReflectionFuncObject()));
    expect_context_status_ok(true, item->invoke<bool>(&object, object));
    string_status status = item->invoke_by_string({"{}"}, &object);
    expect_ok(status);
    EXPECT_EQ("true", status.get());

    EXPECT_FALSE(item->is_static());
    EXPECT_TRUE(item->is_const());
    EXPECT_TRUE(item->is_serializable());
    EXPECT_EQ(1, item->args_size());
    expect_context_status_ok(true, object.invoke<bool>(item->func_name(), ReflectionFuncObject()));
    expect_context_status_ok("true", object.invoke_by_string(item->func_name(), {"{}"}));
  }
  {
    EXPECT_EQ(std::string_view("template_func<int>"), template_int_item.func_name());
    expect_context_status_ok(236, template_int_item.invoke(&object, 3));

    const func_item* item = &object.get_func_info().get_func_item("template_func<int>");
    expect_context_status_ok(236, item->invoke<int>(&object, 3));
    string_status status = item->invoke_by_string({"3"}, &object);
    expect_ok(status);
    EXPECT_EQ("236", status.get());

    EXPECT_FALSE(item->is_static());
    EXPECT_FALSE(item->is_const());
    EXPECT_TRUE(item->is_serializable());
    EXPECT_EQ(1, item->args_size());

    expect_context_status_ok(236, object.invoke<int>(item->func_name(), 3));
    expect_context_status_ok("236", object.invoke_by_string(item->func_name(), {"3"}));
  }
  {
    EXPECT_EQ(std::string_view("template_func<double>"), template_double_item.func_name());
    expect_context_status_ok(236, template_double_item.invoke(&object, 3.0));

    const func_item* item = &template_double_item;
    expect_context_status_ok(236, item->invoke<int>(&object, 3.0));
    string_status status = item->invoke_by_string({"3.0"}, &object);
    expect_ok(status);
    EXPECT_EQ("236", status.get());

    EXPECT_FALSE(item->is_static());
    EXPECT_FALSE(item->is_const());
    EXPECT_TRUE(item->is_serializable());
    EXPECT_EQ(1, item->args_size());
  }
  {
    const func_item& item = object.get_func_info().get_func_item("void_general_func");
    EXPECT_EQ(std::string("void_general_func"), item.func_name());
    expect_ok(item.invoke(&object, 4, 4.0));
    expect_ok(item.invoke_by_string({"4", "4.0"}));

    EXPECT_FALSE(item.is_static());
    EXPECT_FALSE(item.is_const());
    EXPECT_TRUE(item.is_serializable());
    EXPECT_EQ(2, item.args_size());
    expect_ok(object.invoke(item.func_name(), 4, 4.0));
    expect_context_status_ok("", object.invoke_by_string(item.func_name(), {"4", "4.0"}));
  }
  {
    const func_item& item = object.get_func_info().get_func_item("string_call");
    EXPECT_EQ(std::string("string_call"), item.func_name());
    expect_context_status_ok(std::string("hello world!"),
                             item.invoke<std::string>(&object, std::string("hello world!")));
    expect_context_status_ok(std::string("hello world!"), item.invoke_by_string({"hello world!"}));
    // miss explicit string decare.
    expect_error(item.invoke<std::string>(&object, "hello world!"));

    EXPECT_FALSE(item.is_static());
    EXPECT_FALSE(item.is_const());
    EXPECT_TRUE(item.is_serializable());
    EXPECT_EQ(1, item.args_size());
  }
  {
    const func_item& item = object.get_func_info().get_func_item("virtual_public_func");
    EXPECT_EQ(std::string("virtual_public_func"), item.func_name());
    Base base;
    {
      B b;
      C c;
      D d;

      // It is if they are not virtual public
      expect_context_status_ok(1, item.invoke<int>(&object, &base));
      expect_context_status_ok(2, item.invoke<int>(&object, &b));
      expect_context_status_ok(3, item.invoke<int>(&object, &c));
      expect_context_status_ok(4, item.invoke<int>(&object, &d));
    }
    {
      VirtualB b;
      VirtualC c;
      VirtualD d;
      // Can not just call without explicit conversion
      expect_context_status_ok(2, item.invoke<int>(&object, static_cast<Base*>(&b)));
      expect_context_status_ok(3, item.invoke<int>(&object, static_cast<Base*>(&c)));
      expect_context_status_ok(4, item.invoke<int>(&object, dynamic_cast<Base*>(&d)));
    }

    EXPECT_FALSE(item.is_static());
    EXPECT_FALSE(item.is_const());
    EXPECT_FALSE(item.is_serializable());
    EXPECT_EQ(1, item.args_size());
  }
}

#undef declare_func_item
#undef declare_overload_func_item

} // namespace reflection
