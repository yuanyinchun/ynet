#include "tcp_server.h"

class Server: public TcpServer{
 public:
  Server();
  ~Server();
 
 protected:
  virtual int on_connected(Session* session);
  virtual int on_disconnected(Session* session);
  virtual int on_message(Buffer* buffer, Session* session);
  virtual int on_written(Session* session);
};