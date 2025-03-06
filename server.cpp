#include <sys/socket.h>
#include <stdexcept>
#include <cstring>
#include <cerrno>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>

int main() {
    int success = 0;
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        throw std::runtime_error("Socket creation failed:" + std::string(strerror(errno)));
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8080);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    success = bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    if (success == -1) {
        throw std::runtime_error("Could not attach socket to an address:" + std::string(strerror(errno)));
    }

    success = listen(serverSocket, 5);
    if (success == -1) {
        throw std::runtime_error("Could not listen to incoming connections:" + std::string(strerror(errno)));
    }
   
    while (true) {
        int clientSocket = accept(serverSocket, nullptr, nullptr);
        char buffer[1024] = {0};
        int read = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (read == -1) {
            throw std::runtime_error("Failed receive a message:" + std::string(strerror(errno)));
        }
        std::cout << "Message from client: " << buffer << std::endl;
    }
    close(serverSocket);
   }
