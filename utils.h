#ifndef HEADER_H
#define HEADER_H
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

long GetFileSize(std::string);
void sendFile(int, std::string);
void getFile(int, long, std::string);
int connection(int*, int*, int);
#endif
