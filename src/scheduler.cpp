#include "../include/scheduler.h"

#include <iostream>

#include "../include/debug.h"
#include "../include/settings.h"

namespace runtime {

Scheduler::Scheduler() {
#if LOG
  runtime::debug::log(runtime::debug::kDebug, "Scheduler ctor");
#endif
}

Scheduler::~Scheduler() {
#if LOG
  runtime::debug::log(runtime::debug::kDebug, "Scheduler dtor");
#endif
}

}  // namespace runtime
