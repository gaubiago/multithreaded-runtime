#include "../include/thread_pool.h"

#include <cstdint>
#include <iostream>
#include <thread>

#include "../include/fifo_scheduler.h"

namespace runtime {

ThreadPool::ThreadPool(size_t num_workers, std::unique_ptr<Scheduler> scheduler)
    : num_workers_(num_workers),
      scheduler_(std::move(scheduler)),
      running_(false) {
  std::cout << "Constructing ThreadPool..." << std::endl;
}

ThreadPool::~ThreadPool() {
  std::cout << "Destructing ThreadPool..." << std::endl;
}

// Submit a task to the runtime
void ThreadPool::submit(Task task) { scheduler_->enqueue(std::move(task)); }

void ThreadPool::workerLoop(uint32_t worker_id) {
  std::cout << "Worker " << worker_id << " entering pool..." << std::endl;

  while (running_.load()) {
    std::optional<Task> task = scheduler_->dequeue(worker_id);

    if (task.has_value()) {
      task->fn();
    }
  }
}

// Start worker threads
void ThreadPool::start() {
  std::cout << "Starting the worker pool..." << std::endl;

  running_.store(true);

  for (uint32_t i = 0; i < num_workers_; i++) {
    workers_.emplace_back(std::thread(&ThreadPool::workerLoop, this, i));
  }
}

// Stop workers and wait for completion
void ThreadPool::shutdown() {
  std::cout << "Shutting down the worker pool..." << std::endl;

  running_.store(false);

  scheduler_->exit();

  for (int32_t i = 0; i < workers_.size(); ++i) {
    auto& t = workers_[i];

    if (t.joinable()) {
      std::cout << "Worker " << i << " exiting..." << std::endl;

      t.join();
    }
  }
}

}  // namespace runtime
