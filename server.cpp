// Server side C/C++ program to demonstrate Socket
// programming
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sys/stat.h>

#define PORT 8080
#define BLOCKSIZE 1024

long GetFileSize(std::string filename)
{
    struct stat stat_buf;
    int rc = stat(filename.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}

void sendFile(int newsocket, std::string filename)
{
	std::string line;
  	std::ifstream myfile(filename, std::ios::binary);
	char buffer[1024];
	long fileSize = GetFileSize(filename);
	std::string ssize = std::to_string(fileSize);

	send(newsocket, ssize.c_str(), ssize.length(), 0);

	if (myfile.is_open())
	{
		while (myfile)
		{
			myfile.read(buffer, 1024);
			send(newsocket, buffer, 1024, 0);
			std::cout << "gcount" << myfile.gcount() << '\n';
		}
		myfile.close();
	}
	else std::cout << "Unable to open file"; 
}

int main(int argc, char const* argv[])
{
	int server_fd, new_socket, valread;
	if(argc < 2)
		std::cout << "Enter a file name to send";
	std::string filename = argv[1];
	struct sockaddr_in address;
	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = { 0 };
	const char* hello = "Hello from server";

	// Creating socket file descriptor
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR , &opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// Forcefully attaching socket to the port 8080
	if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) 
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	if (listen(server_fd, 3) < 0) 
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
		perror("accept");
		exit(EXIT_FAILURE);
	}
	valread = read(new_socket, buffer, 1024);
	printf("%s\n", buffer);

	// send the file line by line
	//send(new_socket, hello, strlen(hello), 0);
	sendFile(new_socket, filename);
	
	//printf("Hello message sent\n");

	// closing the connected socket
	close(new_socket);
	// closing the listening socket
	shutdown(server_fd, SHUT_RDWR);
	return 0;
}
