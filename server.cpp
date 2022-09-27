// Client side C/C++ program to demonstrate Socket
// programming
#include "socket.h"
#include <cstdio>
#include <cstring>

#define PORT 1234
#define PORT2 1232
#define TEMPNAME "temp.stl"

int main()
{
	Socket *server_sock = new Socket();
	server_sock->bind(PORT);
	server_sock->listen();

	Socket *asock = server_sock->accept();

	asock->recvFile(TEMPNAME, 4*1024); //Read 1024 bytes of the stream
	asock->sendFile(TEMPNAME, 4*1024);
	remove(TEMPNAME );

	asock->close();

}
