#include "ynet/event_loop_pool.h"


using namespace std;

EventLoopPool::EventLoopPool(int sub_num)
{
    main_event_loop_ = new EventLoop();
    for(int i=0; i<sub_num; i++)
    {
        EventLoop* event_loop = new EventLoop();
	    sub_event_loops_.push_back(event_loop);
    }
    curr_sub_pos_ = -1;
}

void EventLoopPool::start()
{
    //start main event loop
    main_event_loop_->start();
    //start sub event loops
    for(int i=0; i<sub_event_loops_.size(); i++)
    {
        sub_threads.push_back(thread(&EventLoopPool::run, this, i));
    }
    
}

void EventLoopPool::run(int i)
{
    EventLoop* event_loop = sub_event_loops_[i];
    event_loop->start();
}

EventLoop* EventLoopPool::get_main_event_loop()
{
    return main_event_loop_;
}

EventLoop* EventLoopPool::get_sub_event_loop()
{
    int next_pos = (++curr_sub_pos_) % sub_event_loops_.size();
    return sub_event_loops_[next_pos];
}