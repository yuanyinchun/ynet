#pragma once

#include <functional>

enum EVENT {
  READ = 0x01,
  WRITE = 0x02
};

class Channel {
 public:
  Channel(int fd, int events, std::function<void(void*)> read_callback, std::function<void(void*)> write_callback, void* data);

  bool is_write_event_enabled();
  void enable_write_event();
  void disable_write_event();
  
 public:
  int fd;
  int events;
  std::function<void(void*)> read_callback;
  std::function<void(void*)> write_callback;
  void* data;
};
