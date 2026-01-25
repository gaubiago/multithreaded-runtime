#include "../include/processor.h"

#include <cassert>
#include <cstdint>
#include <iostream>

#include "../include/settings.h"

namespace runtime {

Processor::Processor(SortFunc sort_func) : sort_func_(std::move(sort_func)){};

void Processor::sort(uint64_t* start, uint64_t* end) { sort_func_(start, end); }

void Processor::stl_sort(uint64_t* start, uint64_t* end) {
  std::sort(start, end);
}

void Processor::partition(uint32_t n) {
  uint64_t part_sz = WORKLOAD_SZ / n;

  for (uint32_t i = 0; i < n; i++) {
    if (i != n - 1) {
      partitions_.emplace_back(Partition(i * part_sz, ((i + 1) * part_sz) - 1));
    } else {
      partitions_.emplace_back(Partition(i * part_sz, WORKLOAD_SZ - 1));
    }
  }

  assert(partitions_.size() == n && "Partitions must be of size n");
}

void Processor::print_partitions() {
  for (uint32_t i = 0; i < partitions_.size(); i++) {
    std::cout << "p" << i << ": " << partitions_.at(i).start << ".."
              << partitions_.at(i).end << std::endl;
  }
}

const std::vector<Partition>& Processor::get_partitions() const {
  return partitions_;
}

};  // namespace runtime