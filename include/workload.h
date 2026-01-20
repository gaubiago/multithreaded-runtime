#include <cstdint>
#include <vector>

namespace runtime {

struct Partition {
  uint64_t start;
  uint64_t end;

  Partition(uint64_t start_, uint64_t end_) : start(start_), end(end_) {}
};

class Workload {
 private:
  std::vector<uint64_t> list_;
  std::vector<Partition> partitions_;

 public:
  explicit Workload(uint64_t sz);
  ~Workload() = default;
  void print();
  void partition(uint32_t n);
  void print_partitions();
};

}  // namespace runtime