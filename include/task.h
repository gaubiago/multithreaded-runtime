#pragma once

#include <chrono>
#include <cstdint>
#include <functional>

namespace runtime {

struct Partition;

using TaskFn = std::function<void()>;

struct Task {
  uint64_t id;
  TaskFn fn;

  // Hint to the scheduler; not a guarantee
  uint32_t estimated_cost;

  // Timestamp when the task was submitted
  std::chrono::steady_clock::time_point submit_time;

  static uint64_t next_id_;

  Task(TaskFn fn_, uint32_t estimated_cost_ = 1)
      : id(next_id_++),
        fn(std::move(fn_)),
        estimated_cost(estimated_cost_),
        submit_time(std::chrono::steady_clock::now()) {}
};

}  // namespace runtime
