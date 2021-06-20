#include "ynet/dispatcher.h"
#include <sys/epoll.h>
#include <iostream>

using namespace std;

Dispatcher::Dispatcher()
{
    efd = epoll_create1(0);
    if(efd < 0)
    {
        cerr << "epoll create error:" << efd << endl;
        exit(-1);
    }
}

Dispatcher::~Dispatcher()
{
    
}

Dispatcher::add_channel(Channel* channel)
{

}

Dispatcher::del_channel(Channel* channel)
{

}

Dispatcher::update_channel(Channel* channel)
{

}

Dispatcher::dispatch(Channel* channel)
{

}