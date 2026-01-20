#include <iostream>

#include "../include/fifo_scheduler.h"
#include "../include/scheduler.h"
#include "../include/workload.h"

int main() {
  auto *scheduler = new runtime::FifoScheduler{};
  scheduler->enqueue(
      runtime::Task(1, []() { std::cout << "Task A" << std::endl; }));
  scheduler->dequeue(0);
  delete scheduler;

  return 0;
}