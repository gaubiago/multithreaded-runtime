#include "../include/processor.h"

#include <cstdint>

namespace runtime {

Processor::Processor(SortFunc sort_func) : sort_func_(std::move(sort_func)){};

void Processor::sort(uint64_t* start, uint64_t* end) { sort_func_(start, end); }

void Processor::stl_sort(uint64_t* start, uint64_t* end) {
  std::sort(start, end);
}

};  // namespace runtime