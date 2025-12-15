#include <iostream>
#include <string>
#include <cstring>
#include <stdexcept>
#include <system_error>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

const int SERVER_PORT = 13;
const int BUFFER_SIZE = 256;

class UdpDaytimeClient {
private:
    int sockfd;
    struct sockaddr_in serv_addr;

public:
    UdpDaytimeClient(const std::string& server_ip) {
        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd < 0) {
            throw std::system_error(errno, std::system_category(), "Ошибка создания сокета");
        }

        memset(&serv_addr, 0, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(SERVER_PORT);

        if (inet_pton(AF_INET, server_ip.c_str(), &serv_addr.sin_addr) <= 0) {
            close(sockfd);
            throw std::runtime_error("Неверный IP-адрес: " + server_ip);
        }
    }

    ~UdpDaytimeClient() {
        if (sockfd >= 0) {
            close(sockfd);
        }
    }

    std::string getDaytime() {
        char buffer[BUFFER_SIZE] = {0};
        socklen_t addr_len = sizeof(serv_addr);

        if (sendto(sockfd, "", 0, 0, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
            throw std::system_error(errno, std::system_category(), "Ошибка отправки запроса");
        }

        ssize_t n = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0,
                             (struct sockaddr*)&serv_addr, &addr_len);
        if (n < 0) {
            throw std::system_error(errno, std::system_category(), "Ошибка получения ответа");
        }

        buffer[n] = '\0';
        return std::string(buffer);
    }
};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Использование: " << argv[0] << " <IP_адрес_сервера>" << std::endl;
        std::cerr << "Пример: " << argv[0] << " 172.16.40.1" << std::endl;
        return EXIT_FAILURE;
    }

    try {
        UdpDaytimeClient client(argv[1]);
        std::string daytime = client.getDaytime();
        std::cout << "Время сервера: " << daytime;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
