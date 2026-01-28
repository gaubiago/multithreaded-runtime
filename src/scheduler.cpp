#include "../include/scheduler.h"

#include <iostream>

#include "../include/debug.h"

namespace runtime {

Scheduler::Scheduler() {
  runtime::debug::log(runtime::debug::kDebug, "Scheduler ctor");
}

Scheduler::~Scheduler() {
  runtime::debug::log(runtime::debug::kDebug, "Scheduler dtor");
}

}  // namespace runtime
