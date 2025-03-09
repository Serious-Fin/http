#ifndef HTTPRESPONSE_H
#define HTTPRESPONSE_H

#include <map>
#include <sstream>
#include <string>

class HttpResponse {
private:
    static int extractFieldFrom(int start, const std::string& data, std::string &result); 
    static int extractHeaders(int start, const std::string& data, std::map<std::string, std::string>& headers);

public:
    std::string version;
    std::string status;
    std::string statusMsg;
    std::map<std::string, std::string> headers;
    std::string body;

    HttpResponse();
    HttpResponse(std::string version, std::string status, std::string statusMsg);
    std::string serialize();
    static HttpResponse deserialize(const std::string& data);
    std::string stringify();
};

#endif
