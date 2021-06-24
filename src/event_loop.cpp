#include "ynet/event_loop.h"
#include <iostream>
#include <sys/socket.h>
#include <unistd.h>

using namespace std;

EventLoop::EventLoop()
{
    quit_ = false;
    dispatcher_ = new Dispatcher();
    owner_thread_id_ = std::this_thread::get_id();

    int ret = socketpair(AF_INET, SOCK_STREAM, 0, socket_pair_);
    if(ret == -1)
    {
        cerr << "create socket pair failed" << endl;
        exit(-1);
    }
}

EventLoop::~EventLoop()
{
    delete dispatcher_;
}

void EventLoop::add_channel(Channel* channel)
{
    process_channel(channel, ChannelOperation::OP::ADD);
}

void EventLoop::del_channel(Channel* channel)
{
    process_channel(channel, ChannelOperation::OP::DEL);
}

void EventLoop::mod_channel(Channel* channel)
{
    process_channel(channel, ChannelOperation::OP::MOD);
}

void EventLoop::process_channel(Channel* channel, ChannelOperation::OP op)
{
    ChannelOperation* chop = new ChannelOperation();
    chop->op = op;
    chop->channel = channel;
    pending_channel_.enqueue(chop);

    if(is_same_thread())
    {
        handle_pending_channel();
    }else
    {
        wake_up();
    }
}

void EventLoop::handle_pending_channel()
{
    while(true)
    {
        ChannelOperation* chop = nullptr;
        bool ret = pending_channel_.try_dequeue(chop);
        if(!ret)
            break;
        
        switch (chop->op)
        {
        case ChannelOperation::OP::ADD:
            dispatcher_->add_channel(chop->channel);
            break;
        case ChannelOperation::OP::DEL:
            dispatcher_->del_channel(chop->channel);
            break;
        case ChannelOperation::OP::MOD:
            dispatcher_->mod_channel(chop->channel);
            break;
        default:
            break;
        }
    }
}

bool EventLoop::is_same_thread()
{
    return std::this_thread::get_id() == owner_thread_id_;
}

void EventLoop::wake_up()
{
    char a = 'a';
    int ret = write(socket_pair_[0], &a, sizeof(a));
    if(ret != sizeof(a))
    {
        cerr << "wake_up failed" << endl;
        exit(-1);
    }
}

void EventLoop::start()
{
    while(!quit_)
    {
        dispatcher_->dispatch();
        handle_pending_channel();
    }

    cout << "eventloop quit" << endl;
}