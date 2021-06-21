#pragma once

#include "ynet/event_loop_pool.h"

class TcpServer {
 public:
  TcpServer();
  ~TcpServer();

  bool start(uint16_t listen_port, int sub_event_loop_num);

 private:
  int create_listen_fd(uint16_t listen_port);
  void listen_fd_read_callback(int fd);
  void connect_fd_read_callback(int fd);
  void connect_fd_write_callback(int fd);

 private:
  EventLoopPool* event_loop_pool_; 
};
