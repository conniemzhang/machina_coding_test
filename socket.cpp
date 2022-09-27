#include "socket.h"
#include <cstdlib>

Socket::Socket(){
    this->opt = 1;
    memset(&address, 0, sizeof address);
	if ((this->sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Socket creation error \n");
	}
}

void Socket::bind(int port){
    if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {

        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);

    this->port = port;
    this->addrlen = sizeof(address);

    // Forcefully attaching socket to the port 8080
    if (::bind(sock, (struct sockaddr*)&address,
             sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
}

void Socket::listen() {
    if (::listen(sock, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }
    std::cout << "listening started\n";
}

Socket* Socket::accept(){
    int new_socket;
    if ((new_socket = ::accept(sock, (struct sockaddr*)&address,
                  (socklen_t*)&addrlen)) < 0) {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    Socket *newSocket = new Socket();
    newSocket->sock = new_socket;
    //newSocket->port = port;
    return newSocket;
}

int Socket::connect(int inport){
    address.sin_family = AF_INET;
    address.sin_port = htons(inport);

    // Convert IPv4 and IPv6 addresses from text to binary
    // form
    if (inet_pton(AF_INET, "127.0.0.1", &address.sin_addr) <= 0) {
        printf(
            "\nInvalid address/ Address not supported \n");
        return -1;
    }
    int status;
    if ((status = ::connect(sock, (struct sockaddr*)&address,
                   sizeof(address))) < 0) {
        printf("\nConnection Failed on %i \n", sock);
        return -1;
    }
    return 1;
}

int Socket::read(std::string &buf,int len) {
    char buffer[len];
    bzero(buffer,len);
    int status = ::read(sock, buffer, len);
    std::cout<<"Read message: " << buffer << "\n";
    if (status < 0) {
        printf("\nerror with reading\n");
        EXIT_FAILURE;
    }
    buf = std::string(buffer);
    return status;
}

int Socket::send(std::string msg){
    int status;
    const char * buf = msg.c_str();
    int len = (int)strlen(buf);
    status = ::send(sock, buf, len + 1, 0);
    if (status < 0) {
        printf("\nSend Failure\n");
        EXIT_FAILURE;
    }
    return status;
}

long Socket::GetFileSize(std::string fi)
{
    struct stat stat_buf;
    int rc = stat(fi.c_str(), &stat_buf);
    return rc == 0 ? stat_buf.st_size : -1;
}

int Socket::recvBuffer(char* buffer, int buf_size, int chunk_size = 4 * 1024) {
    int i = 0;
    while (i < buf_size) {
        const int l = recv(sock, &buffer[i], std::min(chunk_size, buf_size - i), 0);
        if (l < 0) { 
            return l; 
        } // this is an error
        i += l;
    }
    return i;
}

int Socket::sendBuffer(const char* buffer, int buf_size, int chunk_size = 4 * 1024) {
    int i = 0;
    while (i < buf_size) {
        const int l = ::send(sock, &buffer[i], std::min(chunk_size, buf_size - i), 0);
        if (l < 0) { 
            return l; 
        } // this is an error
        i += l;
    }
    return i;
}

long Socket::sendFile(const std::string& fileName, int chunkSize = 64 * 1024) {
    const long fileSize = GetFileSize(fileName);
    std::cout << fileSize << std::endl;

    if (fileSize < 0) { return -1; }

    std::ifstream file(fileName, std::ifstream::binary);
    if (file.fail()) { return -1; }

    if (sendBuffer(reinterpret_cast<const char*>(&fileSize),
        sizeof(fileSize)) != sizeof(fileSize)) {
        return -2;
    }

    char* buffer = new char[chunkSize];
    bool errored = false;
    int64_t i = fileSize;
    while (i != 0) {
        const int64_t ssize = std::min(i, (int64_t)chunkSize);
        if (!file.read(buffer, ssize)) { errored = true; break; }
        const int l = sendBuffer(buffer, (int)ssize);
        if (l < 0) { errored = true; break; }
        i -= l;
    }
    delete[] buffer;

    file.close();

    return errored ? -3 : fileSize;
}

long Socket::recvFile(const std::string& fileName, int chunkSize = 64 * 1024) {
    std::ofstream file(fileName, std::ofstream::binary);
    if (file.fail()) { return -1; }

    int64_t fileSize;
    if (recvBuffer(reinterpret_cast<char*>(&fileSize),
            sizeof(fileSize)) != sizeof(fileSize)) {
        return -2;
    }

    char* buffer = new char[chunkSize];
    bool errored = false;
    int64_t i = fileSize;
    while (i != 0) {
        const int r = recvBuffer(buffer, (int)std::min(i, (int64_t)chunkSize));
        if ((r < 0) || !file.write(buffer, r)) { errored = true; break; }
        i -= r;
    }
    delete[] buffer;

    file.close();

    return errored ? -3 : fileSize;
}

void Socket::close(){
    ::shutdown(sock, SHUT_RDWR);
    ::close(sock);
}
