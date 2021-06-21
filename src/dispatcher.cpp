#include "ynet/dispatcher.h"
#include <iostream>
#include <unistd.h>
#include <cstring>

using namespace std;

Dispatcher::Dispatcher()
{
    epfd = epoll_create1(0);
    if(epfd < 0)
    {
        cerr << "epoll create error:" << epfd << endl;
        exit(-1);
    }

    events_buffer.reserve(events_buffer_size);
}

Dispatcher::~Dispatcher()
{
}

void Dispatcher::add_channel(Channel* channel)
{
    epoll_event e;
    int fd = channel->fd;
    e.data.fd = fd;
    if(channel->events & EVENT::READ)
    	e.events |= EPOLLIN;
    if(channel->events & EVENT::WRITE)
	e.events |= EPOLLOUT;
    int ret = epoll_ctl(epfd,EPOLL_CTL_ADD, fd, &e);
    if(ret == -1)
    {
        cerr << "add_channel error:" << strerror(errno) << endl;
        exit(-1);
    }

    channel_map[fd] = channel;
}

void Dispatcher::del_channel(Channel* channel)
{
    epoll_event e;
    int fd = channel->fd;
    e.data.fd = fd;
    if(channel->events & EVENT::READ)
    	e.events |= EPOLLIN;
    if(channel->events & EVENT::WRITE)
	e.events |= EPOLLOUT;
    int ret = epoll_ctl(epfd,EPOLL_CTL_DEL, fd, &e);
    if(ret == -1)
    {
        cerr << "del_channel error:" << strerror(errno) << endl;
        exit(-1);
    }

    auto itr = channel_map.find(fd);
    if(itr != channel_map.end())
    {
	channel_map.erase(itr);
    }
}

void Dispatcher::mod_channel(Channel* channel)
{
    epoll_event e;
    int fd = channel->fd;
    e.data.fd = fd;
    if(channel->events & EVENT::READ)
    	e.events |= EPOLLIN;
    if(channel->events & EVENT::WRITE)
	e.events |= EPOLLOUT;
    int ret = epoll_ctl(epfd,EPOLL_CTL_MOD, fd, &e);
    if(ret == -1)
    {
        cerr << "mod_channel error:" << strerror(errno) << endl;
        exit(-1);
    }

    channel_map[fd] = channel;
}

void Dispatcher::dispatch()
{
    int n = epoll_wait(epfd, &events_buffer[0], events_buffer_size, 3);
    if(n == -1)
    {
	cerr << "dispatch error:" << strerror(errno) << endl;
        exit(-1);
    }

    for(int i=0; i<n; i++)
    {
	epoll_event* epe = &events_buffer[i];
	int fd = epe->data.fd;

	auto itr = channel_map.find(fd);
	if(itr == channel_map.end())
	    continue;
	Channel* channel = itr->second;

	if(epe->events & EPOLLERR || epe->events & EPOLLHUP)
	{
	    cerr << "epoll error, fd:" << fd << endl;
	    close(fd);
	    continue;
	}

	if(epe->events & EPOLLIN)
	{
	    channel->read_callback(fd);
	}

	if(epe->events & EPOLLOUT)
	{
	    channel->write_callback(fd);
	}
    }
}