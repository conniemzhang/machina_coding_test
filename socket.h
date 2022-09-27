#ifndef SOCKET_H
#define SOCKET_H
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <vector>

class Socket {
    public:
        struct sockaddr_in address;
        int addrlen;
        int sock;
        int port;
        int opt;
        Socket();
        void bind(int port);
        void listen();
        Socket* accept();
        int read(std::string&, int);
        int send(std::string);
        int connect(int);
        int sendBuffer(const char*, int, int);
        int recvBuffer(char *, int, int);
        long sendFile(const std::string& , int);
        long recvFile(const std::string&, int);



        void close();
    protected:
        long getFileSize(std::string);
        long GetFileSize(std::string);

};
#endif