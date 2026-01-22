#include <iostream>

#include "../include/fifo_scheduler.h"
#include "../include/scheduler.h"
#include "../include/settings.h"
#include "../include/workload.h"

int main() {
  runtime::Workload workload(WORKLOAD_SZ);
  workload.print();
  workload.partition(NUM_PARTITIONS);
  workload.print_partitions();
  return 0;
}