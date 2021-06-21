#include "ynet/tcp_server.h"
#include <sys/socket.h>
#include <cstring>
#include <netinet/in.h>
#include <iostream>
#include <fcntl.h>

using namespace std;

TcpServer::TcpServer()
{
}

TcpServer::~TcpServer()
{

}

int TcpServer::create_listen_fd(uint16_t listen_port)
{
    int fd = socket(PF_INET, SOCK_STREAM, 0);
    if(fd == -1)
    {
	cerr << "tcp server start error: " << strerror(errno) << endl;
	exit(-1);
    }

    fcntl(fd, F_SETFL, O_NONBLOCK);

    int on=1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on));

    sockaddr_in addr;
    addr.sin_family = PF_INET;
    addr.sin_port = htons(listen_port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    
    int ret = bind(fd, (sockaddr*)&addr, sizeof(addr));
    if(ret < 0)
    {
        cerr << "bind error: " << strerror(errno) << endl;
        exit(-1);
    }

    ret = listen(fd, 1024);
    if(ret == -1)
    {
        cerr << "listen error: " << strerror(errno) << endl;
        exit(-1);
    }

    return fd;
}


void TcpServer::listen_fd_read_callback(int fd)
{
    sockaddr_in client_addr;
    socklen_t socklen = sizeof(client_addr);
    int connect_fd = accept(fd, (sockaddr*)&client_addr, &socklen);
    if(connect_fd == -1)
    {
	cerr << "accept error: " << strerror(errno) << endl;
	return;
    }

    fcntl(fd, F_SETFL, O_NONBLOCK);

    EventLoop* sub_event_loop = event_loop_pool_->get_sub_event_loop();
    Channel ch;
    ch.fd = fd;
    ch.events |= EVENT::READ;
    ch.events |= EVENT::WRITE;
    ch.read_callback = std::bind(&TcpServer::connect_fd_read_callback, this, std::placeholders::_1);
    ch.write_callback = std::bind(&TcpServer::connect_fd_write_callback, this, std::placeholders::_1);
    sub_event_loop->add_channel(&ch);
}

void TcpServer::connect_fd_read_callback(int fd)
{

}

void TcpServer::connect_fd_write_callback(int fd)
{
	
}

bool TcpServer::start(uint16_t listen_port, int sub_event_loop_num)
{
    int fd = create_listen_fd(listen_port);
    event_loop_pool_ = new EventLoopPool(sub_event_loop_num);
    event_loop_pool_->start();

    EventLoop* main_event_loop = event_loop_pool_->get_main_event_loop();
    Channel ch;
    ch.fd = fd;
    ch.events |= EVENT::READ;
    ch.read_callback = std::bind(&TcpServer::listen_fd_read_callback, this, std::placeholders::_1);
    main_event_loop->add_channel(&ch);

    return true;
}
