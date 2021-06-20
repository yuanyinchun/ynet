#pragma once

#include "ynet/event_loop.h"

class TcpServer {
 public:
  TcpServer();
  ~TcpServer();

  bool start(int port);
  bool stop();

 private:
  EventLoop* event_loop_; 
};
