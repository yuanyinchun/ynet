#pragma once

#include <thread>
#include <vector>
#include "ynet/event_loop.h"

class EventLoopPool {
 public:
  EventLoopPool(int sub_num = 0);
  ~EventLoopPool();
  EventLoop* get_main_event_loop();
  EventLoop* get_sub_event_loop();

  void start();
  
private:
  void run(int i);

 private:
  EventLoop* main_event_loop_;
  std::vector<EventLoop*> sub_event_loops_;
  std::vector<std::thread> sub_threads;
  int curr_sub_pos_;
};