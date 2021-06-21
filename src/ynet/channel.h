#pragma once

#include <functional>

enum EVENT {
  READ,
  WRITE
};

class Channel {
 public:
  int fd;
  int events;
  std::function<void(int)> read_callback;
  std::function<void(int)> write_callback;
};
