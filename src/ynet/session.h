#pragma once

#include "ynet/event_loop.h"
#include "ynet/channel.h"
#include "ynet/buffer.h"
#include <functional>

class Session{
 public:
  Session(int connect_fd, EventLoop* event_loop, 
  	std::function<int(Session*)> on_connected, std::function<int(Session*)> on_disconnected,
	std::function<int(Buffer*, Session*)> on_message, std::function<int(Session*)> on_written);
  ~Session();

  int send_data(void* data, int size);
  int send_buffer(Buffer* buffer);

 public:
  EventLoop* event_loop;
  Channel* channel;
  Buffer* input_buffer;
  Buffer* output_buffer;

  std::function<int(Session*)> on_connected;
  std::function<int(Session*)> on_disconnected;
  std::function<int(Buffer*, Session*)> on_message;
  std::function<int(Session*)> on_written;
  
};