#include "../include/workload.h"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <random>

namespace runtime {

Workload::Workload(uint64_t sz) {
  std::mt19937_64 gen(std::random_device{}());
  std::uniform_int_distribution<uint64_t> dis(0, 100);

  for (uint64_t i = 0; i < sz; i++) {
    list_.push_back(dis(gen));
  }

  assert(list_.size() == sz && "Workload must be of size sz");
}

void Workload::print() {
  for (auto& el : list_) {
    std::cout << el << "  ";
  }
  std::cout << std::endl;
}

void Workload::partition(uint32_t n) {
  uint64_t part_sz = list_.size() / n;

  for (uint32_t i = 0; i < n; i++) {
    if (i != n - 1) {
      partitions_.emplace_back(Partition(i * part_sz, ((i + 1) * part_sz) - 1));
    } else {
      partitions_.emplace_back(Partition(i * part_sz, list_.size() - 1));
    }
  }

  assert(partitions_.size() == n && "Partitions must be of size n");
}

void Workload::print_partitions() {
  for (uint32_t i = 0; i < partitions_.size(); i++) {
    std::cout << "p" << i << ": " << partitions_.at(i).start << ".."
              << partitions_.at(i).end << std::endl;
  }
}

const uint64_t* Workload::get_list_ptr() const { return list_.data(); }

const std::vector<Partition>& Workload::get_partitions() const {
  return partitions_;
}

}  // namespace runtime