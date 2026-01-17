#pragma once

#include <atomic>
#include <memory>
#include <vector>

#include "scheduler.h"

namespace runtime {

class ThreadPool {
public:
  ThreadPool(size_t num_workers, std::unique_ptr<Scheduler> scheduler);

  ~ThreadPool();

  // Non-copyable
  ThreadPool(const ThreadPool &) = delete;
  ThreadPool &operator=(const ThreadPool &) = delete;

  // Submit a task to the runtime
  void submit(Task task);

  // Start worker threads
  void start();

  // Stop workers and wait for completion
  void shutdown();

private:
  void worker_loop(uint32_t worker_id);

  size_t num_workers_;
  std::unique_ptr<Scheduler> scheduler_;

  std::vector<std::thread> workers_;
  std::atomic<bool> running_;
};

} // namespace runtime
