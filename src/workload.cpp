#include "../include/workload.h"

#include <cassert>
#include <cstdint>
#include <iostream>
#include <random>

#include "../include/processor.h"
#include "../include/settings.h"

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

void Workload::print(const std::vector<Partition>& partitions) const {
  const uint64_t* p = get_current_ptr();

  for (const auto& ptn : partitions) {
    std::cout << "[";

    for (uint64_t i = ptn.start; i <= ptn.end; i++) {
      std::cout << *(p + i) << (i != ptn.end ? "  " : "]  ");
    }
  }

  std::cout << std::endl;
}

const uint64_t* Workload::get_current_ptr() const {
  assert(list_[0].source_of_truth != list_[1].source_of_truth &&
         "Workload: There must be only one single source of truth at a time");

  return list_[0].source_of_truth ? list_[0].content.data()
                                  : list_[1].content.data();
}

const uint64_t* Workload::get_stale_ptr() const {
  assert(list_[0].source_of_truth != list_[1].source_of_truth &&
         "Workload: There must be only one single source of truth at a time");

  return !list_[0].source_of_truth ? list_[0].content.data()
                                   : list_[1].content.data();
}

void Workload::refresh_states() {
  list_[0].source_of_truth = !list_[0].source_of_truth;
  list_[1].source_of_truth = !list_[1].source_of_truth;
}

}  // namespace runtime