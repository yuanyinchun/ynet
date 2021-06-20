#pragma once

#include <functional>

class Channel {
 public:
  int fd;
  int events;
  std::function<void()> read_callback;
  std::function<void()> write_callback;
};
