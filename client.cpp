// Client side C/C++ program to demonstrate Socket
// programming
#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <sys/stat.h>

#define PORT 8080

long GetFileSize(std::string filename)
{
    struct stat stat_buf;
    int rc = stat(filename.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}


int main(int argc, char const* argv[])
{
	int sock = 0, valread, client_fd;
	struct sockaddr_in serv_addr;
	const char* hello = "Hello from client";
	char buffer[1024] = { 0 };
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
		return -1;
	}

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(PORT);

	// Convert IPv4 and IPv6 addresses from text to binary form
	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
		printf(
			"\nInvalid address/ Address not supported \n");
		return -1;
	}

	if ((client_fd = connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr))) < 0) 
	{
		printf("\nConnection Failed \n");
		return -1;
	}
	
	send(sock, hello, strlen(hello), 0);
	valread = read(sock, buffer, 1024);

	//convert first message to the fileSize
	long fileSize = std::stol(buffer);
	std::cout << "long : " << fileSize << "\n";
	
	// get in the entire file
  	std::fstream outfile;
	outfile.open ("output.txt", std::ios::binary|std::ios::out);
	if (outfile.is_open())
	{
		while(fileSize > 0)
		{
			long readSize = fileSize > 1024 ? 1024 : fileSize;
			size_t before = outfile.tellp();
			valread = read(sock, buffer, readSize);
			outfile.write(buffer, readSize);
			size_t after = outfile.tellp();
			size_t numBytesWritten = after - before;
			fileSize -= numBytesWritten;
			std::cout << "wrote : " << numBytesWritten << "\n";
		}
		outfile.close();
	}
	else std::cout << "Unable to open file";


	// closing the connected socket
	close(client_fd);
	return 0;
}
