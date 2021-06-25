#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
#include <arpa/inet.h>
#include <iostream>

using namespace std;

int tcp_client(const char *address, int port) {
    int socket_fd;
    socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in server_addr;
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    inet_pton(AF_INET, address, &server_addr.sin_addr);

    socklen_t server_len = sizeof(server_addr);
    int connect_rt = connect(socket_fd, (struct sockaddr *) &server_addr, server_len);
    if (connect_rt < 0) {
	cout << "connect failed" << endl;
    }

    return socket_fd;
}

void test_write(int fd)
{
    int n=10000;
    const char* buf = "abc";
    for(int i=0; i<n; i++)
    {
	ssize_t size = write(fd, buf, 3);
	if(size < 0)
	    cout << "write failed, total count:" << i << endl;
    }

    cout << "total count: " << n << endl;
}

int main()
{
    int fd = tcp_client("127.0.0.1", 8888);
    if(fd < 0 )
	return -1;

    test_write(fd);
}