#include <iostream>
#include <string>
#include <cstring>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

const int PORT = 13;
const int BUFFER_SIZE = 256;

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <server_ip>" << std::endl;
        return 1;
    }

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
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

    if (sendto(sock, "", 0, 0, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        perror("sendto");
        close(sock);
        return 1;
    }

    char buffer[BUFFER_SIZE];
    socklen_t len = sizeof(server_addr);
    ssize_t n = recvfrom(sock, buffer, BUFFER_SIZE-1, 0, (sockaddr*)&server_addr, &len);
    
    if (n < 0) {
        perror("recvfrom");
        close(sock);
        return 1;
    }

    buffer[n] = '\0';
    std::cout << "Server time: " << buffer;

    close(sock);
    return 0;
}
