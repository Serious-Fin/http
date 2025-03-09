#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <sstream>
#include <string>

class HttpRequest {
private:
    static int extractFieldFrom(int start, const std::string& data, std::string &result); 

public:
    std::string method;
    std::string url;
    HttpRequest();
    HttpRequest(std::string method, std::string url);
    std::string serialize();
    static HttpRequest deserialize(const std::string& data);
};

#endif
