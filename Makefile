TARGET=client server test
CC=g++
CFLAGS= -Wall -Wextra -g
normal: $(TARGET)
client: client.cpp
	$(CC) $(CFLAGS) client.cpp -o client
server: server.cpp
	$(CC) $(CFLAGS) server.cpp -o server
test: test.cpp
	$(CC) $(CFLAGS) test.cpp -o test
clean:
	$(RM) $(TARGET)