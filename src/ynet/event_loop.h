#pragma once

#include <list>
#include "ynet/channel.h"
#include "ynet/dispatcher.h"
#include <mutex>
#include "ynet/readerwriterqueue.h"
#include <thread>

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
  void process_channel(Channel* channel, ChannelOperation::OP op);
  void handle_pending_channel();
  bool is_same_thread();
  void wake_up();

 private:
  bool quit_;
  Dispatcher* dispatcher_;
  moodycamel::ReaderWriterQueue<ChannelOperation*> pending_channel_;
  std::thread::id owner_thread_id_;

  int socket_pair_[2];
};
