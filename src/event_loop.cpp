#include "ynet/event_loop.h"
#include <iostream>

using namespace std;

EventLoop::EventLoop()
{
    quit_ = false;
    dispatcher_ = new Dispatcher();
}

EventLoop::~EventLoop()
{
    delete dispatcher_;
}

void EventLoop::add_channel(Channel* channel)
{
    lock_guard<mutex> guard(mtx_);
    ChannelOperation chop;
    chop.op = ChannelOperation::OP::ADD;
    chop.channel = channel;
    pending_channel_.push_back(chop);
}

void EventLoop::del_channel(Channel* channel)
{
    lock_guard<mutex> guard(mtx_);
    ChannelOperation chop;
    chop.op = ChannelOperation::OP::DEL;
    chop.channel = channel;
    pending_channel_.push_back(chop);
}

void EventLoop::mod_channel(Channel* channel)
{
    lock_guard<mutex> guard(mtx_);
    ChannelOperation chop;
    chop.op = ChannelOperation::OP::MOD;
    chop.channel = channel;
    pending_channel_.push_back(chop);
}

void EventLoop::handle_pending_channel()
{
    lock_guard<mutex> guard(mtx_);
    for(auto& chop: pending_channel_)
    {
	switch (chop.op)
	{
	case ChannelOperation::OP::ADD:
		dispatcher_->add_channel(chop.channel);
		break;
	case ChannelOperation::OP::DEL:
		dispatcher_->del_channel(chop.channel);
		break;
	case ChannelOperation::OP::MOD:
		dispatcher_->mod_channel(chop.channel);
		break;
	default:
		break;
	}
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