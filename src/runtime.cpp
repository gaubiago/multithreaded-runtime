#include <chrono>
#include <cstdint>
#include <iostream>

#include "../include/debug.h"
#include "../include/fifo_scheduler.h"
#include "../include/processor.h"
#include "../include/scheduler.h"
#include "../include/settings.h"
#include "../include/task.h"
#include "../include/thread_pool.h"
#include "../include/workload.h"

int main() {
  auto start = std::chrono::high_resolution_clock::now();

  runtime::Workload workload(WORKLOAD_SZ);
  runtime::Processor processor(workload);
  auto scheduler = std::make_unique<runtime::FifoScheduler>();
  runtime::ThreadPool thread_pool(NUM_WORKERS, std::move(scheduler));

#if LOG
  runtime::debug::min_level = runtime::debug::kInfo;

  runtime::debug::log(runtime::debug::kInfo, "Workload:");
  workload.print(processor.get_partitions_info());
#endif

  processor.partition(NUM_PARTITIONS);
  processor.set_sort_algo(runtime::Processor::stl_sort);

  thread_pool.start();

  auto& cur_ptns = const_cast<std::vector<runtime::Partition>&>(
      processor.get_current_ptns_info());
  auto& old_ptns = const_cast<std::vector<runtime::Partition>&>(
      processor.get_stale_ptns_info());

#if LOG
  runtime::debug::log(runtime::debug::kInfo, "Initial partitions:");
  processor.print_partitions();
  workload.print(cur_ptns);
#endif

  std::vector<runtime::Partition> ptns_info = processor.get_partitions_info();

  for (const auto& ptn_info : ptns_info) {
    runtime::Task task([&] { processor.sort_partition(ptn_info); },
                       (ptn_info.end - ptn_info.start) + 1);
    thread_pool.submit(task);
  }

  thread_pool.wait();

#if LOG
  runtime::debug::log(runtime::debug::kInfo, "Sorted partitions");
  processor.print_partitions();
  workload.print(cur_ptns);
#endif

  while (processor.get_num_partitions() > 1) {
    cur_ptns = const_cast<std::vector<runtime::Partition>&>(
        processor.get_current_ptns_info());
    old_ptns = const_cast<std::vector<runtime::Partition>&>(
        processor.get_stale_ptns_info());

    for (uint64_t i = 0; i < processor.get_num_partitions() / 2; i++) {
      runtime::Task task(
          [&, a = cur_ptns[2 * i], b = cur_ptns[2 * i + 1]] {
            processor.merge_partitions(a, b);
          },
          (ptns_info[2 * i + 1].end - ptns_info[2 * i].start) + 1);
      thread_pool.submit(task);
    }

    if (processor.get_num_partitions() % 2 == 1) {
      uint64_t sz = cur_ptns.size();
      runtime::Task task(
          [&, a = cur_ptns[sz - 1]] { processor.copy_left_over_partition(a); },
          (ptns_info[sz - 1].end - ptns_info[sz - 1].start) + 1);
      thread_pool.submit(task);
    }

    thread_pool.wait();
    workload.refresh_states();
    processor.update_partitions_info();

#if LOG
    runtime::debug::log(runtime::debug::kInfo, "Merged partitions:");
    processor.print_partitions();
    cur_ptns = const_cast<std::vector<runtime::Partition>&>(
        processor.get_current_ptns_info());
    workload.print(cur_ptns);
#endif
  }

  thread_pool.shutdown();

  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

  std::cout << "Multi-threaded execution time: " << duration.count() << " ns"
            << std::endl;

  start = std::chrono::high_resolution_clock::now();
  std::vector<uint64_t> dup_workload = workload.get_duplicate();
  std::sort(dup_workload.end(), dup_workload.end());
  end = std::chrono::high_resolution_clock::now();
  duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start);

  std::cout << "Single-threaded execution time: " << duration.count() << " ns"
            << std::endl;

  return 0;
}