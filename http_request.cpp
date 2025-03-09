#include "http_request.h"

int HttpRequest::extractFieldFrom(int start, const std::string& data, std::string &result) {
    int begin = start;
    std::stringstream num;
    while (data[begin] != '$') {
        num << data[begin];
        begin++;
    }
    begin++;
    int length = std::stoi(num.str());
    length = begin + length;
    std::stringstream resStream;
    while (begin < length) {
        resStream << data[begin];
        begin++;
    }
    result = resStream.str();
    return begin;
}

HttpRequest::HttpRequest() {
    method = "";
    url = "";
}

HttpRequest::HttpRequest(std::string _method, std::string _url) {
    method = _method;
    url = _url;
}

std::string HttpRequest::serialize() {
    std::stringstream ss;
    ss << method.length() << "$" << method;
    ss << url.length() << "$" << url;
    return ss.str();
}

 HttpRequest HttpRequest::deserialize(const std::string& data) {
    HttpRequest req;
    std::string result = "";
    int newStart = extractFieldFrom(0, data, result);
    req.method = result;
    newStart = extractFieldFrom(newStart, data, result);
    req.url = result;
    return req;
}

