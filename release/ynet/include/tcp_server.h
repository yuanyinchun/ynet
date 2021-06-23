#pragma once

#include <stdint.h>

class Session;
class Buffer;
class EventLoopPool;

class TcpServer {
 public:
  TcpServer();
  ~TcpServer();

  bool start(uint16_t listen_port, int sub_event_loop_num);

 protected:
  virtual int on_connected(Session* session) = 0;
  virtual int on_disconnected(Session* session) = 0;
  virtual int on_message(Buffer* buffer, Session* session) = 0;
  virtual int on_written(Session* session) = 0;

 private:
  void start_listen_event_loop(uint16_t listen_port, int sub_event_loop_num);
  int create_listen_fd(uint16_t listen_port);
  void listen_fd_read_callback();

 private:
  int listen_fd;
  EventLoopPool* event_loop_pool_; 
};
