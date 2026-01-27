#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>

#include "./scheduler.h"
#include "./workload.h"

namespace runtime {

class FifoScheduler : public Scheduler {
 private:
  std::queue<Task> queue_;
  std::mutex q_mtx_;
  std::condition_variable q_cv_;

  bool terminate_;

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

  void exit() override;
};

}  // namespace runtime
