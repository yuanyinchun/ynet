#pragma once

#include "ynet/channel.h"

class Dispatcher{
 public:
  Dispatcher();
  ~Dispatcher();

  void add_channel(Channel* channel);
  void del_channel(Channel* channel);
  void update_channel(Channel* channel);
  void dispatch();

 private:
  int efd;
};
