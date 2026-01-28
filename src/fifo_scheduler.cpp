#include "../include/fifo_scheduler.h"

#include <iostream>
#include <optional>

#include "../include/task.h"
#include "../include/workload.h"

namespace runtime {

FifoScheduler::FifoScheduler() : terminate_(false) {
  std::cout << "Constructing FIFO Scheduler..." << std::endl;
}

FifoScheduler::~FifoScheduler() {
  std::cout << "Destructing FIFO Scheduler..." << std::endl;
}

void FifoScheduler::enqueue(Task task) {
  std::cout << "Enqueuing Task " << task.id << "..." << std::endl;

  {
    std::lock_guard lk(q_mtx_);
    queue_.push(task);
  }

  q_cv_.notify_one();
}

std::optional<Task> FifoScheduler::dequeue(uint32_t worker_id) {
  std::optional<Task> task;

  {
    std::unique_lock lk(q_mtx_);
    q_cv_.wait(lk, [this]() { return (!queue_.empty() || terminate_); });

    if (!terminate_) {
      task = queue_.front();

      std::cout << "Worker " << worker_id << " dequeuing Task " << task->id
                << "..." << std::endl;

      queue_.pop();
    }
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