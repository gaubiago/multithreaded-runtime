#pragma once

#include <cstdint>
#include <vector>

namespace runtime {

class Workload {
 private:
  std::vector<uint64_t> list_;

 public:
  explicit Workload(uint64_t sz);
  ~Workload() = default;
  void print();
  const uint64_t* get_list_ptr() const;
};

}  // namespace runtime