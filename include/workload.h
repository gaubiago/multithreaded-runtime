#pragma once

#include <cstdint>
#include <vector>

namespace runtime {

class Workload {
 private:
  struct List {
    bool source_of_truth;
    std::vector<uint64_t> content;
  };

  List list_[2];

 public:
  struct Pointer {
    uint64_t* cur;
    uint64_t* old;
  };

  explicit Workload(uint64_t sz);
  ~Workload() = default;
  void print();
  const uint64_t* get_current_list_ptr() const;
  const uint64_t* get_stale_list_ptr() const;
  void refresh_list_states();
};

}  // namespace runtime