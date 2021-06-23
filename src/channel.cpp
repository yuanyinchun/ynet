 #include "ynet/channel.h"
 #include "ynet/session.h"

Channel::Channel(int fd, int events, std::function<void(void*)> read_callback, std::function<void(void*)> write_callback, void* data)
{
    fd = fd;
    events = events;
    read_callback = read_callback;
    write_callback = write_callback;
    data = data;
}

bool Channel::is_write_event_enabled()
{
    return events & EVENT::WRITE;
}

void Channel::enable_write_event()
{
    Session* session = (Session*)data;
    EventLoop* event_loop = session->event_loop;
    events |= EVENT::WRITE;
    event_loop->mod_channel(this);
}

void Channel::disable_write_event()
{
    Session* session = (Session*)data;
    EventLoop* event_loop = session->event_loop;
    events &= ~EVENT::WRITE;
    event_loop->mod_channel(this);
}