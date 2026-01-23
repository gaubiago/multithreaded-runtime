#pragma once

#include <algorithm>
#include <cstdint>
#include <functional>

namespace runtime {

using SortFunc = std::function<void(uint64_t*, uint64_t*)>;

class Processor {
 private:
  SortFunc sort_func_;

 public:
  explicit Processor(SortFunc sort_func);
  ~Processor() = default;
  void sort(uint64_t* start, uint64_t* end);
  static void stl_sort(uint64_t* start, uint64_t* end);
};

}  // namespace runtime