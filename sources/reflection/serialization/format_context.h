#pragma once

namespace reflection {

struct format_context {

  // options for text serialization
  int tab_space_count = 4;

  // In the list, if value_type is trival, then how many size lines are
  int trivial_value_new_line_size = 10;

  int tab_size = 0;
};

} // namespace reflection