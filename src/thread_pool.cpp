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
void ThreadPool::submit(Task task) {
  in_flight_.fetch_add(1, std::memory_order_relaxed);
  scheduler_->enqueue(std::move(task));
}

void ThreadPool::workerLoop(uint32_t worker_id) {
  std::cout << "Worker " << worker_id << " entering pool..." << std::endl;

  while (running_.load()) {
    std::optional<Task> task = scheduler_->dequeue(worker_id);

    if (task.has_value()) {
      task->fn();

      if (in_flight_.fetch_sub(1, std::memory_order_acq_rel) == 1) {
        std::unique_lock lk(done_mtx_);
        done_cv_.notify_one();
      }
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

void ThreadPool::wait() {
  std::unique_lock lk(done_mtx_);
  done_cv_.wait(
      lk, [this]() { return in_flight_.load(std::memory_order_acquire) == 0; });
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
