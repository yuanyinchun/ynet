#pragma once

#include <list>
#include "ynet/channel.h"
#include "ynet/dispatcher.h"
#include <mutex>

class ChannelOperation
{
public:
  enum class OP
  {
    ADD,
    DEL,
    MOD
  };

  OP op;
  Channel* channel;
};

class EventLoop {
 public:
  EventLoop();
  ~EventLoop();

  void add_channel(Channel* channel);
  void del_channel(Channel* channel);
  void mod_channel(Channel* channel);

  void start();

 private:
  void handle_pending_channel();

 private:
  bool quit_;
  Dispatcher* dispatcher_;
  std::vector<ChannelOperation> pending_channel_;
  std::mutex mtx_;
};
