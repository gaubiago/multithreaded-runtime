#include <cstdint>
#include <iostream>

#include "../include/fifo_scheduler.h"
#include "../include/processor.h"
#include "../include/scheduler.h"
#include "../include/settings.h"
#include "../include/workload.h"

int main() {
  runtime::Workload workload(WORKLOAD_SZ);
  runtime::Processor processor(workload);

  std::cout << "Workload:" << std::endl;
  workload.print(processor.get_partitions_info());
  std::cout << std::endl;

  processor.partition(NUM_PARTITIONS);

  processor.set_sort_algo(runtime::Processor::stl_sort);

  processor.sort();

  while (processor.get_num_partitions() > 1) {
    processor.merge();
  }

  // auto* scheduler = new runtime::FifoScheduler();

  return 0;
}