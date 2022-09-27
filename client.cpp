// Client side C/C++ program to demonstrate Socket
// programming
#include "socket.h"
#include <cstdio>
#include <cstring>

#define PORT 1234
#define PORT2 1232
#define FILENAME "cad_mesh.stl"
#define OUTFILENAME "output.stl"

using namespace std;

long GetFileSize(std::string filename)
{
    struct stat stat_buf;
    int rc = stat(filename.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}

int main()
{
	Socket *cli_sock = new Socket();
	cli_sock->connect(PORT);
	
	std::string line;
	long fileSize = GetFileSize(FILENAME);
	cout<<fileSize<<endl;

	cli_sock->sendFile(FILENAME, 1024);
	cli_sock->recvFile(OUTFILENAME, 1024);
	
}
