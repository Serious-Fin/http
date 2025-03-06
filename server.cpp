#include <string>
#include <sys/socket.h>
#include <stdexcept>
#include <cstring>
#include <cerrno>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>
#include <map>
#include <cctype>
#include <bits/stdc++.h>

int createSocket(long address = INADDR_ANY, int port = 8080);
std::string handleHTTP(char* message);

int main() {
    int serverFd = createSocket(); 
    while (true) {
        int clientSocket = accept(serverFd, nullptr, nullptr);
        char buffer[1024] = {0};
        int read = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (read == -1) {
            throw std::runtime_error("Failed receive a message:" + std::string(strerror(errno)));
        }
        // std::cout << "Message from client: " << handleHTTP(buffer) << std::endl;
        std::string resp = handleHTTP(buffer);
        int sent = send(clientSocket, resp.c_str(), resp.length(), 0);
        if (sent == -1) {
            throw std::runtime_error("Failed sending response:" + std::string(strerror(errno)));
        }
    }
    close(serverFd);
}

int createSocket(long address, int port) {
    int success = 0;
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        throw std::runtime_error("Socket creation failed:" + std::string(strerror(errno)));
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = address;

    success = bind(serverSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    if (success == -1) {
        throw std::runtime_error("Could not attach socket to an address:" + std::string(strerror(errno)));
    }

    success = listen(serverSocket, 5);
    if (success == -1) {
        throw std::runtime_error("Could not listen to incoming connections:" + std::string(strerror(errno)));
    }
    return serverSocket;
}

std::string handleHTTP(char* message) {
    std::string request = message;
    if (request.length() < 4) {
        return "Request is too short";
    }
    std::string rs = request.substr(0, 4);
    transform(rs.begin(), rs.end(), rs.begin(), ::tolower);
    if (rs.compare("get ") != 0) {
        return "Request is incorrectly formatted";
    }
    
    std::string parsedRequest = request.substr(4);
    std::map<std::string, std::string> pageMaps{
        {
            "/", "<h1>Home page</h1>"
        },
        {
            "/login", "<h1>Login page</h1>"
        },
        {
            "/courses", "<h1>All courses</h1>"
        }
    };

    if (auto search = pageMaps.find(parsedRequest); search != pageMaps.end()) {
        return search->second;
    } else {
       return "<h1>404 Page not found</h1>"; 
    }
}
