#pragma once

#include "ynet/channel.h"
#include <map>
#include <vector>
#include <sys/epoll.h>

class Dispatcher {
 public:
  Dispatcher();
  ~Dispatcher();

  void add_channel(Channel* channel);
  void del_channel(Channel* channel);
  void mod_channel(Channel* channel);
  void dispatch();

 private:
  int epfd;
  std::map<int, Channel*> channel_map;
  
  std::vector<epoll_event> events_buffer;
  const int events_buffer_size = 128;
};
