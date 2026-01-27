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
  struct Partitions {
    bool source_of_truth;
    std::vector<Partition> content;
  };

  SortFunc sort_func_;
  Workload& workload_;
  Partitions partitions_[2];
  void sort(uint64_t* start, uint64_t* end);

 public:
  explicit Processor(Workload& workload);
  ~Processor() = default;
  static void stl_sort(uint64_t* start, uint64_t* end);
  void set_sort_algo(SortFunc sort_func);
  const std::vector<Partition>& get_current_ptns_info() const;
  const std::vector<Partition>& get_stale_ptns_info() const;
  void refresh_states();
  void partition(uint32_t n);
  void print_partitions() const;
  const std::vector<Partition>& get_partitions_info() const;
  uint64_t get_num_partitions() const;
  void sort_partition(const Partition& a);
  void sort();
  void merge_partitions(const Partition& a, const Partition& b);
  void copy_left_over_partition(const Partition& a);
  void update_partitions_info();
  void merge();
};

}  // namespace runtime