#include <cstdint>
#include <iostream>

#include "../include/fifo_scheduler.h"
#include "../include/processor.h"
#include "../include/scheduler.h"
#include "../include/settings.h"
#include "../include/workload.h"

int main() {
  runtime::Workload workload(WORKLOAD_SZ);
  workload.print();

  runtime::Processor processor(runtime::Processor::stl_sort);
  processor.partition(NUM_PARTITIONS);
  processor.print_partitions();
  const uint64_t* p = workload.get_list_ptr();
  for (const auto& partition : processor.get_partitions()) {
    processor.sort(const_cast<uint64_t*>(p + partition.start),
                   const_cast<uint64_t*>(p + partition.end + 1));
  }
  workload.print();

  // auto* scheduler = new runtime::FifoScheduler();

  return 0;
}