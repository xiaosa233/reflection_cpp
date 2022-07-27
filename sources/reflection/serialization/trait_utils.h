#pragma once

#include <optional>
#include <vector>


namespace reflection {

class meta_object;

/* Usage for selector:
   selector<0, T1, T2, T3>::type -> T1
   selector<1, T1, T2, T3>::type -> T2
 */
template <size_t Index, class T, class... Args>
struct selector {
  using type = typename selector<Index - 1, Args...>::type;
};

template <class T, class... Args>
struct selector<0, T, Args...> {
  using type = T;
};

template <class T>
struct is_vector {
  static constexpr bool value = false;
};

template <class... Args>
struct is_vector<std::vector<Args...>> {
  static constexpr bool value = true;
};

template <class T>
struct is_optional {
  static constexpr bool value = false;
};

template <class T>
struct is_optional<std::optional<T>> {
  static constexpr bool value = true;
};

template <class T>
struct reflected_type {
  enum type_enum { object, vector, optional, general };

  static constexpr type_enum value =
      std::is_convertible_v<T*, meta_object*>
          ? type_enum::object
          : (is_vector<T>::value
                 ? type_enum::vector
                 : (is_optional<T>::value ? type_enum::optional : type_enum::general));
};

template <size_t Index, class... Args>
struct args_type {
  using type = typename std::tuple_element<Index, std::tuple<Args...>>::type;
};


//template<class T>
//struct is_std_container {
//    static constexpr bool value = false;
//};
//
//template<class T, class ... Args>
//struct is_std_container<std::vector<T, Args ...>> {
//    static constexpr bool value = true;
//};
//
//template<class T>
//struct container_name {
//    static_assert(false, "Not support get name for T!");
//};
//
//template <class T>
//struct container_name {
//  static_assert(false, "Not support get name for T!");
//};

} // namespace reflection