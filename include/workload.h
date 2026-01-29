#pragma once

#include <cstdint>
#include <vector>

namespace runtime {

struct Partition;

class Workload {
 private:
  struct List {
    bool source_of_truth;
    std::vector<uint64_t> content;
  };

  List list_[2];
  std::vector<uint64_t> duplicate_;

 public:
  struct Pointer {
    uint64_t* cur;
    uint64_t* old;
  };

  explicit Workload(uint64_t sz);
  ~Workload() = default;
  void print(const std::vector<Partition>& partitions) const;
  const uint64_t* get_current_ptr() const;
  const uint64_t* get_stale_ptr() const;
  void refresh_states();
  std::vector<uint64_t> get_duplicate() const;
};

}  // namespace runtime