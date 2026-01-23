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
  workload.partition(NUM_PARTITIONS);
  workload.print_partitions();

  const uint64_t* p = workload.get_list_ptr();
  runtime::Processor processor(runtime::Processor::stl_sort);
  for (const auto& partition : workload.get_partitions()) {
    processor.sort(const_cast<uint64_t*>(p + partition.start),
                   const_cast<uint64_t*>(p + partition.end));
  }
  workload.print();

  // auto* scheduler = new runtime::FifoScheduler();

  return 0;
}