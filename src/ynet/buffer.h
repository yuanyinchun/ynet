#pragma once

#include <string>
#include <vector>

class Session;

class Buffer{
 public:
  Buffer();
  ~Buffer();
  
  int get_writeable_size();
  int get_readable_size();
  int get_front_spare_size();
  void append(void* data, int size);
  void append_char(char data);
  void append_string(const std::string& str);
  int read_socket(int fd);
 
 private:
  void ensure_buffer_write(int write_size); 

 public:
  std::vector<char> bf;
  int read_index;
  int write_index;

  friend class Session;
};