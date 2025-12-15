#include <iostream>
#include <string>
#include <cstring>
#include <stdexcept>
#include <system_error>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

const int SERVER_PORT = 7;
const int BUFFER_SIZE = 256;

class TcpEchoClient {
private:
    int sockfd;
    struct sockaddr_in serv_addr;

public:
    TcpEchoClient(const std::string& server_ip) {
        sockfd = socket(AF_INET, SOCK_STREAM, 0);
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

        if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
            close(sockfd);
            throw std::system_error(errno, std::system_category(), "Ошибка подключения к серверу");
        }
    }

    ~TcpEchoClient() {
        if (sockfd >= 0) {
            close(sockfd);
        }
    }

    std::string sendAndReceive(const std::string& message) {
        if (send(sockfd, message.c_str(), message.size(), 0) < 0) {
            throw std::system_error(errno, std::system_category(), "Ошибка отправки сообщения");
        }

        char buffer[BUFFER_SIZE] = {0};
        ssize_t n = recv(sockfd, buffer, BUFFER_SIZE - 1, 0);
        if (n < 0) {
            throw std::system_error(errno, std::system_category(), "Ошибка получения ответа");
        }

        buffer[n] = '\0';
        return std::string(buffer);
    }
};

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Использование: " << argv[0] << " <IP_адрес_сервера> \"сообщение\"" << std::endl;
        std::cerr << "Пример: " << argv[0] << " 172.16.40.1 \"Hello, server!\"" << std::endl;
        return EXIT_FAILURE;
    }

    try {
        TcpEchoClient client(argv[1]);
        std::string echo = client.sendAndReceive(argv[2]);
        std::cout << "Эхо-ответ: " << echo << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
