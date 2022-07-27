
#pragma once

#include <memory>
#include <unordered_map>

#include "meta/meta_info_spec.h"
#include "meta/meta_base_classes.h"

namespace reflection {

class meta_info_manager {
public:
  static const meta_info_manager& get() { return inst(); }

  std::unique_ptr<meta_object> create(const std::string& name) const;

  bool has(const std::string& name) const { return meta_info_map_.count(name); }

private:
  static meta_info_manager& inst();
  void push(const std::string& name, const meta_info& value) { meta_info_map_[name] = &value; }

  std::unordered_map<std::string, const meta_info*> meta_info_map_;

  friend class meta_info;
};
} // namespace reflection
