#include "utils.h"

#include <filesystem>
#include <fstream>
#include <limits>

namespace fs = std::filesystem;

namespace reflection {

namespace {

#ifdef _MSC_VER
#pragma pack(push, 1)
#endif

struct file_header {
  unsigned int endian_value : 1;
  unsigned int reserve_value : 31;
}

#ifdef _MSC_VER
#pragma pack(pop)
#else
#ifdef __GNUC__
__attribute__((__packed__))
#endif
#endif
;

constexpr uint32_t magic_number = 0x95279523;
} // namespace

std::string object_to_string(const meta_object& object, format_context* context) {
  std::string result;
  if (object.serialize_to_string(&result, context)) {
    return result;
  }
  return "";
}

status serialize_to_text_file(const meta_object& object, const std::string_view& file_path,
                              bool overwrite, format_context context) {
  if (!overwrite && fs::exists(file_path)) {
    return status::error(view_to_string(file_path) + " already exists.");
  }
  fs::path parent_v = fs::path(file_path).parent_path();
  if (!fs::exists(parent_v) && !fs::create_directories(parent_v)) {
    return status::error("Create directories error : " + parent_v.string());
  }

  std::string out;
  return_if_error(object.serialize_to_string(&out, &context));
  std::ofstream fstream_v(file_path);
  fstream_v.write(out.data(), out.size());
  fstream_v << '\n';
  return !fstream_v.fail()
             ? status::ok()
             : status::error("Failed to write string content to file!" + view_to_string(file_path));
}

status serialize_from_text_file(const std::string_view& file_path, meta_object* object) {
  if (!fs::exists(file_path)) {
    return status::error(view_to_string(file_path) + " does not exists!");
  }
  std::ifstream fstream_v(file_path);
  fstream_v.seekg(0, std::ios::end);
  size_t max_size = static_cast<size_t>(fstream_v.tellg());
  std::string out;
  out.resize(max_size);
  fstream_v.seekg(0, std::ios::beg);
  fstream_v.read(out.data(), out.size());

  return_if_error(object->serialize_from_string(out));
  return status::ok();
}

status serialize_to_binary_file(const meta_object& object, const std::string_view& file_path,
                                endian endian_v, bool overwrite) {
  if (!overwrite && fs::exists(file_path)) {
    return status::error(view_to_string(file_path) + " already exists.");
  }
  fs::path parent_v = fs::path(file_path).parent_path();
  if (!fs::exists(parent_v) && !fs::create_directories(parent_v)) {
    return status::error("Create directories error : " + parent_v.string());
  }

  std::vector<char> buffer;
  return_if_error(object.serialize_to_bytes(&buffer, endian_v));
  std::ofstream fstream_v(file_path, std::ios::binary);

  uint32_t tmp_magic_number = magic_number;
  if (endian::native != endian::big) {
    byte_swapper<sizeof(uint32_t)>::swap(&tmp_magic_number);
  }
  // write magic number
  fstream_v.write(reinterpret_cast<char*>(&tmp_magic_number), sizeof(uint32_t));

  // Offset 4 bytes , reserve for meta info
  // The most 1 bit record the endian value : 0 for little, 1 for big.
  static_assert(sizeof(file_header) == 4);

  file_header header;
  header.endian_value = static_cast<unsigned int>(get_real_endian(endian_v));
  header.reserve_value = 0;
  if (endian::native != endian::big) {
    byte_swapper<4>::swap(&header);
  }
  fstream_v.write(reinterpret_cast<char*>(&header), 4);

  fstream_v.write(buffer.data(), buffer.size());
  return status::ok();
}

status serialize_from_binary_file(const std::string_view& file_path, meta_object* object) {
  if (!fs::exists(file_path)) {
    return status::error(view_to_string(file_path) + " does not exists!");
  }
  std::ifstream fstream_v(file_path, std::ios::binary);
  // check magic number;
  uint32_t in_magic_number = 0;
  fstream_v.read(reinterpret_cast<char*>(&in_magic_number), 4);
  if (endian::native != endian::big) {
    byte_swapper<4>::swap(&in_magic_number);
  }
  if (in_magic_number != magic_number) {
    return status::error(
        "Magic number is not match!. May be the file is not a serialized binary file.");
  }
  // Read 4 bytes meta offset.
  file_header header;
  fstream_v.read(reinterpret_cast<char*>(&header), 4);
  if (endian::native != endian::big) {
    byte_swapper<4>::swap(&header);
  }
  endian endian_v = static_cast<endian>(header.endian_value);
  std::vector<char> buffer;
  fstream_v.seekg(0, std::ios::end);
  size_t max_size = static_cast<size_t>(fstream_v.tellg()) - 8;
  buffer.resize(max_size);
  fstream_v.seekg(8, std::ios::beg);
  fstream_v.read(buffer.data(), buffer.size());
  return_if_error(
      object->serialize_from_bytes(buffer.data(), buffer.data() + buffer.size(), endian_v));
  return status::ok();
}



} // namespace reflection
