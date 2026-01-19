#include <cstdint>

namespace runtime {

struct Work {
  uint64_t start_pos;
  uint64_t end_pos;
  uint32_t worker_id;
  uint32_t estimated_cost;

  Work(uint64_t start_pos_, uint64_t end_pos_, uint32_t worker_id_,
       uint32_t estimated_cost_)
      : start_pos(start_pos_),
        end_pos(end_pos_),
        worker_id(worker_id_),
        estimated_cost(estimated_cost_) {}
};

}  // namespace runtime