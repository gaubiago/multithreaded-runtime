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

  runtime::Processor processor(runtime::Processor::stl_sort, workload);
  processor.partition(NUM_PARTITIONS);
  processor.print_partitions();

  processor.sort_partition(processor.get_partitions()[0]);
  processor.sort_partition(processor.get_partitions()[1]);

  workload.print();

  processor.merge_partitions(processor.get_partitions()[0],
                             processor.get_partitions()[1]);

  workload.print();

  // auto* scheduler = new runtime::FifoScheduler();

  return 0;
}