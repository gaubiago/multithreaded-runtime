#include "../include/thread_pool.h"

#include <cstdint>
#include <iostream>
#include <thread>

#include "../include/debug.h"
#include "../include/fifo_scheduler.h"
#include "../include/settings.h"

namespace runtime {

ThreadPool::ThreadPool(size_t num_workers, std::unique_ptr<Scheduler> scheduler)
    : num_workers_(num_workers),
      scheduler_(std::move(scheduler)),
      running_(false) {
#if LOG
  runtime::debug::log(runtime::debug::kDebug, "ThreadPool ctor");
#endif
}

ThreadPool::~ThreadPool() {
#if LOG
  runtime::debug::log(runtime::debug::kDebug, "ThreadPool dtor");
#endif
}

// Submit a task to the runtime
void ThreadPool::submit(Task task) {
  in_flight_.fetch_add(1, std::memory_order_relaxed);
  scheduler_->enqueue(std::move(task));
}

void ThreadPool::workerLoop(uint32_t worker_id) {
#if LOG
  runtime::debug::log_worker(worker_id, runtime::debug::kDebug,
                             "Entering worker pool");
#endif

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
#if LOG
  runtime::debug::log(runtime::debug::kDebug, "Starting worker pool");
#endif

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
#if LOG
  runtime::debug::log(runtime::debug::kDebug, "Shutting down worker pool");
#endif

  running_.store(false);

  scheduler_->exit();

  for (int32_t i = 0; i < workers_.size(); ++i) {
    auto& t = workers_[i];

    if (t.joinable()) {
#if LOG
      runtime::debug::log_worker(i, runtime::debug::kDebug, "Exiting");
#endif

      t.join();
    }
  }
}

}  // namespace runtime
