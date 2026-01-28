#include <cstdint>
#include <iostream>

#include "../include/fifo_scheduler.h"
#include "../include/processor.h"
#include "../include/scheduler.h"
#include "../include/settings.h"
#include "../include/task.h"
#include "../include/workload.h"
#include "thread_pool.h"

int main() {
  runtime::Workload workload(WORKLOAD_SZ);
  runtime::Processor processor(workload);

  std::cout << "Workload:" << std::endl;
  workload.print(processor.get_partitions_info());
  std::cout << std::endl;

  processor.partition(NUM_PARTITIONS);

  processor.set_sort_algo(runtime::Processor::stl_sort);

  auto scheduler = std::make_unique<runtime::FifoScheduler>();

  runtime::ThreadPool thread_pool(NUM_WORKERS, std::move(scheduler));
  thread_pool.start();

  auto& cur_ptns = const_cast<std::vector<runtime::Partition>&>(
      processor.get_current_ptns_info());
  auto& old_ptns = const_cast<std::vector<runtime::Partition>&>(
      processor.get_stale_ptns_info());
  std::cout << "Initial partitions:" << std::endl;
  processor.print_partitions();
  workload.print(cur_ptns);
  std::cout << std::endl;

  std::vector<runtime::Partition> ptn_info = processor.get_partitions_info();

  for (uint64_t i = 0; i < ptn_info.size(); i++) {
    runtime::Task task(
        i, [&, ptn_info = ptn_info[i]] { processor.sort_partition(ptn_info); },
        (ptn_info[i].end - ptn_info[i].start) + 1);
    thread_pool.submit(task);
  }

  thread_pool.wait();

  std::cout << "Sorted partitions:" << std::endl;
  processor.print_partitions();
  workload.print(cur_ptns);
  std::cout << std::endl;

  while (processor.get_num_partitions() > 1) {
    cur_ptns = const_cast<std::vector<runtime::Partition>&>(
        processor.get_current_ptns_info());
    old_ptns = const_cast<std::vector<runtime::Partition>&>(
        processor.get_stale_ptns_info());

    for (uint64_t i = 0; i < processor.get_num_partitions() / 2; i++) {
      runtime::Task task(
          i,
          [&, a = cur_ptns[2 * i], b = cur_ptns[2 * i + 1]] {
            processor.merge_partitions(a, b);
          },
          (ptn_info[2 * i + 1].end - ptn_info[2 * i].start) + 1);
      thread_pool.submit(task);
    }

    if (processor.get_num_partitions() % 2 == 1) {
      uint64_t sz = cur_ptns.size();
      runtime::Task task(
          sz - 1,
          [&, a = cur_ptns[sz - 1]] { processor.copy_left_over_partition(a); },
          (ptn_info[sz - 1].end - ptn_info[sz - 1].start) + 1);
      thread_pool.submit(task);
    }

    thread_pool.wait();

    workload.refresh_states();

    std::cout << "Merged partitions:" << std::endl;
    processor.update_partitions_info();
    processor.print_partitions();
    cur_ptns = const_cast<std::vector<runtime::Partition>&>(
        processor.get_current_ptns_info());
    workload.print(cur_ptns);
    std::cout << std::endl;
  }

  thread_pool.shutdown();

  return 0;
}