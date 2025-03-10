#include <string>
#include <sys/socket.h>
#include <stdexcept>
#include <cstring>
#include <cerrno>
#include <netinet/in.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include "http_request.h"
#include "http_response.h"

int createSocket(long address = INADDR_ANY, int port = 8080);
void sendHTTP(const char* message);
std::string createHttpRequest(std::string message);

int main() { 
    std::string message;
    while (true) {
        std::cout << "Message: ";
        std::getline(std::cin, message);
        std::string req = createHttpRequest(message);
        const char* convertedReq = req.c_str();
        sendHTTP(convertedReq);
    }
    return 0;
}

std::string createHttpRequest(std::string message) {
    HttpRequest req("POST", message, "HTTP/1.0");
    req.headers["Authentication"] = "Finnish-Fish";
    req.headers["User-Agent"] = "NCSA_Mosaic/2.0 (Windows 3.1)";
    req.body = "<Request Body>";
    return req.serialize();
}

void sendHTTP(const char* message) {
    int clientSocket = createSocket();
    try {
        int sent = send(clientSocket, message, strlen(message), 0);
        if (sent == -1) {
            throw (errno);
        }
    }
    catch (int errorNumber) {
        throw std::runtime_error("Could not send message:" + std::string(strerror(errorNumber)));
    }

    char buffer[1024] = {0}; 
    int received = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (received == -1) {
        throw std::runtime_error("Failed getting response:" + std::string(strerror(errno)));
    }
    HttpResponse resp = HttpResponse::deserialize(buffer);
    std::cout << "Resp: " << resp.stringify() << "\n";
    close(clientSocket);
}

int createSocket(long address, int port) {
    int socketFd = socket(AF_INET, SOCK_STREAM, 0);
    if (socketFd == -1) {
        throw std::runtime_error("Socket create error:" + std::string(strerror(errno)));
    }

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(port);
    serverAddress.sin_addr.s_addr = address;
    int success = connect(socketFd, (struct sockaddr*)&serverAddress, sizeof(serverAddress));
    if (success == -1) {
        throw std::runtime_error("Socket binding to address fail:" + std::string(strerror(errno)));
    }
    return socketFd;
}
