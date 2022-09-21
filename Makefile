TARGET=client server 
CC=gcc
CFLAGS= -Wall -Wextra -g -std=c++11
normal: $(TARGET)
client: client.cpp
	$(CC) $(CFLAGS) client.cpp -o client
server: server.cpp
	$(CC) $(CFLAGS) server.cpp -o server
clean:
	$(RM) $(TARGET)