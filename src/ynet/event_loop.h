#pragma once

#include <list>
#include "ynet/channel.h"


class EventLoop {
 public:
  EventLoop();
  ~EventLoop();

  void add_channel(Channel* channel);
  void del_channel(Channel* channel);
  void update_channel(Channel* channel);

  void start();

 private:
  void handle_pending_channel();

 private:
  std::list<Channel*> pending_channel_;
};
