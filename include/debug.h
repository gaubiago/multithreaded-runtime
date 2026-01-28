// include/debug.h
#pragma once
#include <iostream>
#include <string>

namespace runtime::debug {

enum Level { kTrace, kDebug, kInfo, kWarn, kError, kCritical };

// Set this to control which messages are displayed
inline Level min_level = kInfo;

inline void log(Level level, const std::string& message) {
  if (level < min_level) return;  // Skip if below threshold

  const char* level_str[] = {"[TRACE]", "[DEBUG]", "[INFO]",
                             "[WARN]",  "[ERROR]", "[CRITICAL]"};
  std::cout << level_str[level] << " " << message << std::endl;
}

inline void log_worker(uint32_t worker_id, Level level,
                       const std::string& action) {
  log(level, "Worker " + std::to_string(worker_id) + ": " + action);
}

inline void log_task(uint64_t task_id, Level level, const std::string& action) {
  log(level, "Task " + std::to_string(task_id) + ": " + action);
}
}  // namespace runtime::debug