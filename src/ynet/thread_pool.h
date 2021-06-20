#pragma once

#include <thread>
#include <vector>

class ThreadPool {
 public:
  ThreadPool(int thread_num);
  ~ThreadPool();

  std::thread get_thread();

 private:
  std::vector<std::thread> threads;
  int current_position;
};