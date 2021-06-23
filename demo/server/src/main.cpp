#include "server.h"

int main()
{
    Server server;
    server.start(9999, 2);
    return 0;
}