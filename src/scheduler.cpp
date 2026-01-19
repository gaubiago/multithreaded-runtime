#include "../include/scheduler.h"

#include <iostream>

namespace runtime {

Scheduler::Scheduler() {
  std::cout << "Constructing Scheduler..." << std::endl;
}

Scheduler::~Scheduler() {
  std::cout << "Destructing Scheduler..." << std::endl;
}

}  // namespace runtime
