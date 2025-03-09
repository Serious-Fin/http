#include <asm-generic/socket.h>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <stdexcept>
#include <cstring>
#include <cerrno>
#include <netinet/in.h>
#include <unistd.h>
#include <map>
#include <cctype>
#include <bits/stdc++.h>
#include "http_request.h"
#include "http_request.h"
#include "http_response.h"

int createSocket(long address = INADDR_ANY, int port = 8080);
HttpResponse handleHTTP(HttpRequest req);

int main() {
    int serverFd = createSocket(); 
    while (true) {
        int clientSocket = accept(serverFd, nullptr, nullptr);
        char buffer[1024] = {0};
        int read = recv(clientSocket, buffer, sizeof(buffer), 0);
        if (read == -1) {
            throw std::runtime_error("Failed receive a message:" + std::string(strerror(errno)));
        }
        std::string stringReq = buffer;
        HttpRequest req = HttpRequest::deserialize(stringReq);
        std::cout << "Got request: " << req.stringify() << "\n";
        HttpResponse resp = handleHTTP(req);
        std::string respSerialized = resp.serialize();
        int sent = send(clientSocket, respSerialized.c_str(), respSerialized.length(), 0);
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

    int opt = 1;
    if (setsockopt(serverSocket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        throw std::runtime_error("Failed to set socket options:" + std::string(strerror(errno)));
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

HttpResponse handleHTTP(HttpRequest req) {
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

    std::string responseBody = "";
    if (auto search = pageMaps.find(req.url); search != pageMaps.end()) {
        responseBody = search->second;
    } else {
        responseBody = "<h1>404 Page not found</h1>"; 
    }
    HttpResponse resp("HTTP 1.0", "200", "OK");
    resp.body = responseBody;
    return resp; 
}
