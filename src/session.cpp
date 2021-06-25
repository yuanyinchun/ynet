#include "ynet/session.h"
#include <iostream>
#include <unistd.h>

using namespace std;

void handle_closed(Session* session)
{
    EventLoop* event_loop = session->event_loop;
    event_loop->del_channel(session->channel);
    session->on_disconnected(session);
}

void handle_read(void* data)
{
    Session* session = (Session*)data;
    Buffer* input_buffer = session->input_buffer;
    Channel* channel = session->channel;

    int ret = input_buffer->read_socket(channel->fd);
    if(ret<=0)
    {
        cerr << "handle_read error" << endl;
        handle_closed(session);
    }

    session->on_message(input_buffer, session);
}

void handle_write(void* data)
{
    Session* session = (Session*)data;
    Buffer* output_buffer = session->output_buffer;
    Channel* channel = session->channel;

    ssize_t ret = write(channel->fd, &output_buffer->bf[output_buffer->read_index], output_buffer->get_readable_size());
    if(ret>0)
    {
        output_buffer->read_index += ret;
        if(output_buffer->get_readable_size() == 0)
        {
            cout << "write complete" << endl;
            channel->disable_write_event();
        }

        session->on_written(session);
    }
}

Session::Session(int connect_fd, EventLoop* event_loop, 
  	std::function<int(Session*)> on_connected, std::function<int(Session*)> on_disconnected,
	std::function<int(Buffer*, Session*)> on_message, std::function<int(Session*)> on_written)
{
    event_loop = event_loop;
    on_connected = on_connected;
    on_disconnected = on_disconnected;
    on_message = on_message;
    on_written = on_written;
    input_buffer = new Buffer();
    output_buffer = new Buffer();

    channel = new Channel(connect_fd, EVENT::READ, handle_read, handle_write, this);
    event_loop->add_channel(channel);
    on_connected(this);
}

Session::~Session()
{
    delete input_buffer;
    delete output_buffer;
}

int Session::send_data(void* data, size_t size)
{
    if(!data)
	    return -1;

    int written_size = 0;
    if(output_buffer->get_readable_size() == 0 && !channel->is_write_event_enabled())
    {
        written_size = write(channel->fd, data, size);
        if(written_size<0)
        {
            written_size = 0;
        }
    }

    int left_size = size - written_size;
    if(left_size > 0)
    {
        output_buffer->append(data + written_size, left_size);
        if(!channel->is_write_event_enabled())
            channel->enable_write_event();
    }
}

int Session::send_buffer(Buffer* buffer)
{
    int size = buffer->get_readable_size();
    int ret = send_data(&buffer->bf[buffer->read_index], size);
    buffer->read_index += size;
    return ret;
}
