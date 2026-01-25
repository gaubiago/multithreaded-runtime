#include "../include/workload.h"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <random>

namespace runtime {

Workload::Workload(uint64_t sz) {
  std::mt19937_64 gen(std::random_device{}());
  std::uniform_int_distribution<uint64_t> dis(0, 100);

  list_[0].source_of_truth = true;
  list_[1].source_of_truth = false;

  for (uint64_t i = 0; i < sz; i++) {
    list_[0].content.push_back(dis(gen));
  }

  list_[1].content = list_[0].content;

  assert(list_[0].content.size() == sz && "Workload must be of size sz");
}

void Workload::print() {
  uint64_t i = 0;

  for (auto idx : {0, 1}) {
    std::cout << idx << ": ";

    for (auto& el : list_[idx].content) {
      // std::cout << "[" << i++ << "]:";
      std::cout << el << "  ";
    }

    std::cout << std::endl;
  }
}

const uint64_t* Workload::get_current_list_ptr() const {
  return list_[0].source_of_truth ? list_[0].content.data()
                                  : list_[1].content.data();
}

const uint64_t* Workload::get_stale_list_ptr() const {
  return !list_[0].source_of_truth ? list_[0].content.data()
                                   : list_[1].content.data();
}

void Workload::refresh_list_states() {
  if (list_[0].source_of_truth) {
    list_[0].source_of_truth = false;
    list_[1].source_of_truth = true;
  } else {
    list_[0].source_of_truth = true;
    list_[1].source_of_truth = false;
  }
}

}  // namespace runtime