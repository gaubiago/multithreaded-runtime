#pragma once

#include "./scheduler.h"

namespace runtime {

class FifoScheduler : Scheduler {
 public:
  FifoScheduler();

  ~FifoScheduler() override;

  // Submit a new task into the scheduler
  void enqueue(Task task) override;

  // Attempt to get a task for execution
  // Returns empty if no task is available
  std::optional<Task> dequeue(uint32_t worker_id) override;

  // Number of pending tasks (approximate is acceptable)
  size_t size() const override;
};

}  // namespace runtime
