#pragma once

#include <string>

namespace reflection {

#define return_if_error(value)                                                                     \
  if (!value)                                                                                      \
  return value

#define expect_ok(value)                                                                           \
  {                                                                                                \
    const auto tmp = value;                                                                        \
    EXPECT_TRUE(tmp) << #value << " message is " << tmp.message();                                 \
  }                                                                                                \
  while (false)

#define expect_error(value)                                                                        \
  {                                                                                                \
    const auto tmp = value;                                                                        \
    EXPECT_FALSE(tmp) << #value << " message is " << tmp.message();                                \
  }                                                                                                \
  while (false)

class status {
public:
  explicit status(int exec_code, const std::string& message = "")
      : exec_code_(exec_code), message_(message) {}

  bool operator==(const status& other) const { return exec_code_ == other.exec_code_; }

  operator bool() const { return exec_code_ == 0; }

  const std::string& message() const { return message_; }
  int code() const { return exec_code_; }

  static status error(const std::string& message = "") {
    status error_inst(-1, message);
    return error_inst;
  }

  static status ok() {
    static status ok_inst(0);
    return ok_inst;
  }

private:
  int exec_code_ = 0;
  std::string message_;
};

template <class T>
class context_status : public status {
public:
  explicit context_status(int exec_code, const std::string& message = "")
      : status(exec_code, message) {}

  context_status(const status& status_v) : status(status_v.code(), status_v.message()) {}

  const T& get() const { return value_; }

  static context_status ok(const T& value) {
    context_status result(0);
    result.value_ = value;
    return result;
  }

protected:
  T value_;
};

// if the T is void, we will use bool to avid compile error.
template <>
class context_status<void> : public context_status<bool> {
public:
  context_status() : context_status<bool>(0) { value_ = false; }
  explicit context_status(int exec_code, const std::string& message = "")
      : context_status<bool>(exec_code, message) {
    value_ = false;
  }

  context_status(const status& status_v)
      : context_status<bool>(status_v.code(), status_v.message()) {}
};

using string_status = context_status<std::string>;

} // namespace reflection
