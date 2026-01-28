#pragma once

#include <atomic>
#include <cstdint>
#include <thread>
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

  void wait();

  // Stop workers and wait for completion
  void shutdown();

 private:
  void workerLoop(uint32_t worker_id);

  size_t num_workers_;
  std::unique_ptr<Scheduler> scheduler_;

  std::vector<std::thread> workers_;
  std::atomic<bool> running_;

  std::atomic<uint64_t> in_flight_{0};
  std::mutex done_mtx_;
  std::condition_variable done_cv_;
};

}  // namespace runtime
