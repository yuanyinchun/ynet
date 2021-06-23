#include "ynet/buffer.h"
#include <cstring>
#include <sys/uio.h>

#define INIT_BUFFER_SIZE 65536

Buffer::Buffer()
{
    bf.reserve(INIT_BUFFER_SIZE);
    read_index = 0;
    write_index = 0;
}

Buffer::~Buffer()
{
}

int Buffer::get_writeable_size()
{
    return bf.size() - write_index;
}

int Buffer::get_readable_size()
{
    return write_index - read_index;
}

int Buffer::get_front_spare_size()
{
    return read_index;
}

void Buffer::ensure_buffer_write(int write_size)
{
    if(get_writeable_size() >= write_size)
	return;

    if((get_front_spare_size() + get_writeable_size()) >= write_size)
    {
	int readable_size = get_readable_size();
	std::copy(bf.begin()+read_index, bf.begin()+write_index, bf.begin());
	read_index = 0;
	write_index = readable_size;
    }else
    {
	bf.reserve(bf.size() + write_size);
    }
}

void Buffer::append(void* data, int size)
{
    if(data)
    {
	ensure_buffer_write(size);
	memcpy(&bf[write_index], data, size);
	write_index += size;
    }
}

void Buffer::append_char(char c)
{
    ensure_buffer_write(1);
    bf[write_index++] = c;
}

void Buffer::append_string(const std::string& str)
{
    append((void*)str.c_str(), str.size());
}

int Buffer::read_socket(int fd)
{
    std::vector<char> additional_buffer;
    additional_buffer.reserve(INIT_BUFFER_SIZE);

    int max_writable_size = get_writeable_size();
    iovec vec[2];
    vec[0].iov_base = &bf[write_index];
    vec[0].iov_len = max_writable_size;
    vec[1].iov_base = &additional_buffer[0];
    vec[1].iov_len = INIT_BUFFER_SIZE;

    int ret = readv(fd, vec, 2);
    if(ret<0)
	return -1;
    
    if(ret <= max_writable_size)
    {
	write_index += ret;
    }else
    {
	write_index = bf.size()-1;
	append(&additional_buffer[0], ret - max_writable_size);
    }

    return ret;
}
