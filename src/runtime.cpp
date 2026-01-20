#include <iostream>

#include "../include/fifo_scheduler.h"
#include "../include/scheduler.h"
#include "../include/workload.h"

#define WORKLOAD_SZ 25
#define NUM_PARTITIONS 4

int main() {
  runtime::Workload workload(WORKLOAD_SZ);
  workload.print();
  workload.partition(NUM_PARTITIONS);
  workload.print_partitions();

  auto *scheduler = new runtime::FifoScheduler{};
  scheduler->enqueue(
      runtime::Task(1, []() { std::cout << "Task A" << std::endl; }));
  scheduler->dequeue(0);
  delete scheduler;

  return 0;
}