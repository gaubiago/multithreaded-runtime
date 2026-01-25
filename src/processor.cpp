#include "../include/processor.h"

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <iostream>

#include "../include/settings.h"

namespace runtime {

Processor::Processor(SortFunc sort_func, Workload& workload)
    : sort_func_(std::move(sort_func)), workload_(workload){};

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

void Processor::sort_partition(const Partition& a) {
  const uint64_t* ptr = workload_.get_current_list_ptr();

  sort(const_cast<uint64_t*>(ptr + a.start),
       const_cast<uint64_t*>(ptr + a.end + 1));
}

void Processor::merge_partitions(const Partition& a, const Partition& b) {
  runtime::Workload::Pointer p;
  p.cur = const_cast<uint64_t*>(workload_.get_current_list_ptr());
  p.old = const_cast<uint64_t*>(workload_.get_stale_list_ptr());

  uint64_t i = a.start;
  uint64_t j = b.start;

  uint64_t k = std::min(i, j);

  while (i != a.end + 1 && j != b.end + 1) {
    if (*(p.cur + j) <= *(p.cur + i)) {
      *(p.old + k) = *(p.cur + j);
      j++;
    } else {
      *(p.old + k) = *(p.cur + i);
      i++;
    }

    k++;
  }

  while (i != a.end + 1) {
    *(p.old + k) = *(p.cur + i);
    i++;
    k++;
  }

  while (j != b.end + 1) {
    *(p.old + k) = *(p.cur + j);
    j++;
    k++;
  }
}

};  // namespace runtime