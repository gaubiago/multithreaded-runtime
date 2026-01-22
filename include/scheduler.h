#pragma once

#include <optional>

#include "./task.h"

namespace runtime {

class Scheduler {
 public:
  Scheduler();

  virtual ~Scheduler();

  // Submit a new task into the scheduler
  virtual void enqueue(Task task) = 0;

  // Attempt to get a task for execution
  // Returns empty if no task is available
  virtual std::optional<Task> dequeue(uint32_t worker_id) = 0;

  // Number of pending tasks (approximate is acceptable)
  virtual size_t size() const = 0;

  virtual void exit() = 0;
};

}  // namespace runtime
