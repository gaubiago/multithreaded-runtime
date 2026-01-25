#pragma once

#include <algorithm>
#include <cstdint>
#include <functional>
#include <vector>

#include "workload.h"

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
  Workload& workload_;
  std::vector<Partition> partitions_;
  void sort(uint64_t* start, uint64_t* end);

 public:
  explicit Processor(SortFunc sort_func, Workload& workload);
  ~Processor() = default;
  static void stl_sort(uint64_t* start, uint64_t* end);
  void partition(uint32_t n);
  void print_partitions();
  const std::vector<Partition>& get_partitions() const;
  void sort_partition(const Partition& a);
  void merge_partitions(const Partition& a, const Partition& b);
};

}  // namespace runtime