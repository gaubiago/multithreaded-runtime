#include "../include/processor.h"

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <system_error>

#include "../include/settings.h"
#include "workload.h"

namespace runtime {

Processor::Processor(Workload& workload) : workload_(workload) {
  sort_func_ = nullptr;

  partitions_[0].source_of_truth = true;
  partitions_[0].content.emplace_back(Partition(0, WORKLOAD_SZ - 1));

  partitions_[1].source_of_truth = false;
  partitions_[1].content = partitions_[0].content;
};

void Processor::sort(uint64_t* start, uint64_t* end) { sort_func_(start, end); }

void Processor::stl_sort(uint64_t* start, uint64_t* end) {
  std::sort(start, end);
}

void Processor::set_sort_algo(SortFunc sort_func) {
  sort_func_ = std::move(sort_func);
}

const std::vector<Partition>& Processor::get_current_ptns_info() const {
  assert(partitions_[0].source_of_truth != partitions_[1].source_of_truth &&
         "Processor: There must be only one single source of truth at a time");

  return partitions_[0].source_of_truth ? partitions_[0].content
                                        : partitions_[1].content;
}

const std::vector<Partition>& Processor::get_stale_ptns_info() const {
  assert(partitions_[0].source_of_truth != partitions_[1].source_of_truth &&
         "Processor: There must be only one single source of truth at a time");

  return !partitions_[0].source_of_truth ? partitions_[0].content
                                         : partitions_[1].content;
}

void Processor::refresh_states() {
  partitions_[0].source_of_truth = !partitions_[0].source_of_truth;
  partitions_[1].source_of_truth = !partitions_[1].source_of_truth;
}

void Processor::partition(uint32_t n) {
  uint64_t ptn_sz = WORKLOAD_SZ / n;
  auto& partitions =
      const_cast<std::vector<Partition>&>(get_current_ptns_info());
  partitions.clear();

  for (uint32_t i = 0; i < n; i++) {
    if (i != n - 1) {
      partitions.emplace_back(Partition(i * ptn_sz, ((i + 1) * ptn_sz) - 1));
    } else {
      partitions.emplace_back(Partition(i * ptn_sz, WORKLOAD_SZ - 1));
    }
  }

  assert(partitions.size() == n && "Partitions must be of size n");
}

void Processor::print_partitions() const {
  const auto& partitions = get_current_ptns_info();

  for (uint32_t i = 0; i < partitions.size(); i++) {
    std::cout << "p" << i << ": " << partitions.at(i).start << ".."
              << partitions.at(i).end << std::endl;
  }
}

const std::vector<Partition>& Processor::get_partitions_info() const {
  return get_current_ptns_info();
}

uint64_t Processor::get_num_partitions() const {
  const auto& partitions = get_current_ptns_info();
  return partitions.size();
}

void Processor::sort_partition(const Partition& a) {
  if (!sort_func_) {
    throw std::logic_error(
        "Processor::sort_partition(): sort function not set. "
        "Call set_sort_algo() before attempting to sort.");
  }

  const uint64_t* ptr = workload_.get_current_ptr();

  sort(const_cast<uint64_t*>(ptr + a.start),
       const_cast<uint64_t*>(ptr + a.end + 1));
}

void Processor::merge_partitions(const Partition& a, const Partition& b) {
  runtime::Workload::Pointer p;
  p.cur = const_cast<uint64_t*>(workload_.get_current_ptr());
  p.old = const_cast<uint64_t*>(workload_.get_stale_ptr());

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

void Processor::update_partitions_info() {
  const auto& cur_ptns = get_current_ptns_info();
  auto& old_ptns = const_cast<std::vector<Partition>&>(get_stale_ptns_info());
  const uint64_t num_ptns = get_num_partitions();

  old_ptns.clear();

  for (uint64_t i = 0; i < num_ptns / 2; i++) {
    old_ptns.emplace_back(
        Partition(cur_ptns[2 * i].start, cur_ptns[2 * i + 1].end));
  }

  if (num_ptns % 2 == 1) {
    old_ptns.emplace_back(cur_ptns[num_ptns - 1]);
  }

  refresh_states();
}

void Processor::sort_and_merge_partitions() {
  auto& cur_ptns = const_cast<std::vector<Partition>&>(get_current_ptns_info());
  auto& old_ptns = const_cast<std::vector<Partition>&>(get_stale_ptns_info());

  std::cout << "Initial partitions:" << std::endl;
  print_partitions();
  workload_.print(cur_ptns);
  std::cout << std::endl;

  for (const auto& ptn : cur_ptns) {
    sort_partition(ptn);
  }

  std::cout << "Sorted partitions:" << std::endl;
  print_partitions();
  workload_.print(cur_ptns);
  std::cout << std::endl;

  for (uint64_t i = 0; i < get_num_partitions() / 2; i++) {
    merge_partitions(cur_ptns[2 * i], cur_ptns[2 * i + 1]);
  }
  workload_.refresh_states();

  std::cout << "Merged partitions:" << std::endl;
  update_partitions_info();
  print_partitions();
  cur_ptns = const_cast<std::vector<Partition>&>(get_current_ptns_info());
  workload_.print(cur_ptns);
  std::cout << std::endl;

  if (cur_ptns.size() == 1) {
    std::cout << "Sorted workload:" << std::endl;
    print_partitions();
    workload_.print(cur_ptns);
    std::cout << std::endl;
  }
}

};  // namespace runtime