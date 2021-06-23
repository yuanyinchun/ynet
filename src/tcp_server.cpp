#include "ynet/tcp_server.h"

#include "ynet/event_loop_pool.h"
#include "ynet/session.h"

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


void TcpServer::listen_fd_read_callback()
{
    sockaddr_in client_addr;
    socklen_t socklen = sizeof(client_addr);
    int connect_fd = accept(listen_fd, (sockaddr*)&client_addr, &socklen);
    if(connect_fd == -1)
    {
        cerr << "accept error: " << strerror(errno) << endl;
        return;
    }

    fcntl(listen_fd, F_SETFL, O_NONBLOCK);

    EventLoop* sub_event_loop = event_loop_pool_->get_sub_event_loop();
    Session* session = new Session(connect_fd, sub_event_loop, 
                    std::bind(&TcpServer::on_connected, this, std::placeholders::_1), 
                    std::bind(&TcpServer::on_disconnected, this, std::placeholders::_1),
                    std::bind(&TcpServer::on_message, this, std::placeholders::_1, std::placeholders::_2), 
                    std::bind(&TcpServer::on_written, this, std::placeholders::_1)
                    );
}

void TcpServer::start(uint16_t listen_port, int sub_event_loop_num)
{
    //create and start sub event loop pool
    event_loop_pool_ = new EventLoopPool(sub_event_loop_num);
    event_loop_pool_->start();

    //start main event loop
    start_main_event_loop(listen_port, sub_event_loop_num);
}

void TcpServer::start_main_event_loop(uint16_t listen_port, int sub_event_loop_num)
{
    int listen_fd = create_listen_fd(listen_port);

    EventLoop* main_event_loop = event_loop_pool_->get_main_event_loop();

    int events;
    events |= EVENT::READ;
    Channel* channel = new Channel(listen_fd, events, std::bind(&TcpServer::listen_fd_read_callback, this), nullptr, nullptr);
    
    main_event_loop->add_channel(channel);

    main_event_loop->start();
}