#include "server.h"
#include <iostream>

using namespace std;

int Server::on_connected(Session* session)
{
    cout << "on_connected" << endl;
}

int Server::on_disconnected(Session* session)
{
    cout << "on_disconnected" << endl;
}

int Server::on_message(Buffer* buffer, Session* session)
{
    cout << "on_message" << endl;
}

int Server::on_written(Session* session)
{
    cout << "on_written" << endl;	
}