#include <iostream>
#include <string>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

const int PORT = 7;
const int BUFFER_SIZE = 256;

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <server_ip> \"message\"" << std::endl;
        return 1;
    }

    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("socket");
        return 1;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    
    if (inet_pton(AF_INET, argv[1], &server_addr.sin_addr) <= 0) {
        std::cerr << "Invalid IP address" << std::endl;
        close(sock);
        return 1;
    }

    if (connect(sock, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("connect");
        close(sock);
        return 1;
    }

    std::string msg = argv[2];
    if (send(sock, msg.c_str(), msg.size(), 0) < 0) {
        perror("send");
        close(sock);
        return 1;
    }

    char buffer[BUFFER_SIZE];
    ssize_t n = recv(sock, buffer, BUFFER_SIZE-1, 0);
    
    if (n < 0) {
        perror("recv");
        close(sock);
        return 1;
    }

    buffer[n] = '\0';
    std::cout << "Echo: " << buffer << std::endl;

    close(sock);
    return 0;
}
