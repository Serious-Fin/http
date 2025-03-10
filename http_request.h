#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

#include <map>
#include <sstream>
#include <string>

class HttpRequest {
private:
    static int extractFieldFrom(int start, const std::string& data, std::string &result); 
    static int extractHeaders(int start, const std::string& data, std::map<std::string, std::string>& headers);

public:
    std::string method;
    std::string url;
    std::string version;
    std::map<std::string, std::string> headers;
    std::string body;

    HttpRequest();
    HttpRequest(std::string method, std::string url, std::string version);
    std::string serialize();
    static HttpRequest deserialize(const std::string& data);
    std::string stringify();
};

#endif
