#include "../include/fifo_scheduler.h"

#include <iostream>
#include <mutex>
#include <optional>

#include "../include/debug.h"
#include "../include/task.h"
#include "../include/workload.h"

namespace runtime {

FifoScheduler::FifoScheduler() : terminate_(false) {
  runtime::debug::log(runtime::debug::kDebug, "FifoScheduler ctor");
}

FifoScheduler::~FifoScheduler() {
  runtime::debug::log(runtime::debug::kDebug, "FifoScheduler dtor");
}

void FifoScheduler::enqueue(Task task) {
  runtime::debug::log_task(task.id, runtime::debug::kDebug, "Enqueued");

  {
    std::lock_guard lk(q_mtx_);
    queue_.push(task);
  }

  q_cv_.notify_one();
}

std::optional<Task> FifoScheduler::dequeue(uint32_t worker_id) {
  std::optional<Task> task;

  std::unique_lock lk(q_mtx_);
  q_cv_.wait(lk, [this]() { return (!queue_.empty() || terminate_); });

  if (!terminate_) {
    task = queue_.front();

    runtime::debug::log_worker(worker_id, runtime::debug::kDebug,
                               "Dequeued Task " + std::to_string(task->id));

    queue_.pop();
  }

  return task;
}

size_t FifoScheduler::size() {
  std::lock_guard lk(q_mtx_);
  return queue_.size();
}

void FifoScheduler::exit() {
  terminate_ = true;
  q_cv_.notify_all();
}

}  // namespace runtime