#include "../include/task.h"

namespace runtime {

uint64_t Task::next_id_ = 0;

Task::Task(TaskFn fn_, uint32_t estimated_cost_)
    : id(next_id_++),
      fn(std::move(fn_)),
      estimated_cost(estimated_cost_),
      submit_time(std::chrono::steady_clock::now()) {}

}  // namespace runtime