#include "meta/meta_manager.h"

namespace reflection {

meta_info_manager& meta_info_manager::inst() {
  static meta_info_manager _inst;
  return _inst;
}

std::unique_ptr<meta_object> meta_info_manager::create(const std::string& name) const {
  auto it = meta_info_map_.find(name);
  if (it == meta_info_map_.end())
    return {};
  return it->second->create();
}

} // namespace reflection