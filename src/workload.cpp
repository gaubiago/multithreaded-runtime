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

const uint64_t* Workload::get_list_ptr() const { return list_.data(); }

}  // namespace runtime