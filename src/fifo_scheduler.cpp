#include "../include/fifo_scheduler.h"

#include <iostream>
#include <optional>

#include "../include/settings.h"
#include "../include/task.h"
#include "../include/workload.h"

namespace runtime {

FifoScheduler::FifoScheduler() : workload_(Workload(WORKLOAD_SZ)) {
  std::cout << "START: Constructing FIFO Scheduler..." << std::endl;
  workload_.print();
  workload_.partition(NUM_PARTITIONS);
  workload_.print_partitions();
  std::cout << "END: Constructing FIFO Scheduler..." << std::endl;
}

FifoScheduler::~FifoScheduler() {
  std::cout << "Destructing FIFO Scheduler..." << std::endl;
}

void FifoScheduler::enqueue(Task task) {
  std::cout << "Enqueuing:" << std::endl;
  task.fn();
}

std::optional<Task> FifoScheduler::dequeue(uint32_t worker_id) {
  std::cout << "Dequeuing:" << std::endl;
  Task task(1, [worker_id]() {
    std::cout << "Task executed by Worker " << worker_id << std::endl;
  });
  task.fn();
  return std::optional<Task>(task);
}

size_t FifoScheduler::size() const { return 0; }

}  // namespace runtime