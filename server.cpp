#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main() {
    int serverSock = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSock == -1) {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(12345);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(serverSock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        std::cerr << "Error binding socket" << std::endl;
        return 1;
    }

    if (listen(serverSock, 5) == -1) {
        std::cerr << "Error listening on socket" << std::endl;
        return 1;
    }

    std::cout << "Server listening..." << std::endl;

    sockaddr_in clientAddr;
    socklen_t clientAddrSize = sizeof(clientAddr);
    int clientSock = accept(serverSock, (struct sockaddr*)&clientAddr, &clientAddrSize);

    char message[1024];
    while (true) {
        ssize_t bytesRead = recv(clientSock, message, sizeof(message), 0);
        if (bytesRead <= 0) {
            std::cerr << "Client disconnected" << std::endl;
            break;
        }

        message[bytesRead] = '\0';
        std::cout << "Client: " << message << std::endl;

        if (strcmp(message, "exit") == 0)
            break;
    }

    close(clientSock);
    close(serverSock);
    return 0;
}
