#pragma once

#include <algorithm>
#include <cstdint>
#include <functional>
#include <vector>

namespace runtime {

using SortFunc = std::function<void(uint64_t*, uint64_t*)>;

struct Partition {
  uint64_t start;
  uint64_t end;

  Partition(uint64_t start_, uint64_t end_) : start(start_), end(end_) {}
};

class Processor {
 private:
  SortFunc sort_func_;
  std::vector<Partition> partitions_;

 public:
  explicit Processor(SortFunc sort_func);
  ~Processor() = default;
  void sort(uint64_t* start, uint64_t* end);
  static void stl_sort(uint64_t* start, uint64_t* end);
  void partition(uint32_t n);
  void print_partitions();
  const std::vector<Partition>& get_partitions() const;
};

}  // namespace runtime