TARGET=client server test
CC=g++
CFLAGS= -Wall -Wextra -g
normal: $(TARGET)
client: client.cpp socket.cpp
	$(CC) $(CFLAGS) client.cpp socket.cpp -o client
server: server.cpp socket.cpp
	$(CC) $(CFLAGS) server.cpp socket.cpp -o server
test: test.cpp
	$(CC) $(CFLAGS) test.cpp -o test
clean:
	$(RM) $(TARGET)