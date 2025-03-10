#include "http_request.h"
#include <sstream>
#include <string>

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

int HttpRequest::extractHeaders(int start, const std::string& data, std::map<std::string, std::string>& headers) {
    int begin = start;
    std::stringstream num;
    while (data[begin] != '$') {
        num << data[begin];
        begin++;
    }
    begin++;
    
    std::string headerKey = "";
    std::string headerValue = "";
    int headerCount = std::stoi(num.str());
    for (int i = 0; i < headerCount; i++) {
        begin = extractFieldFrom(begin, data, headerKey);
        begin = extractFieldFrom(begin, data, headerValue);
        headers[headerKey] = headerValue;
    }
    return begin;
}

HttpRequest::HttpRequest() {
    method = "";
    url = "";
    version = "";
    headers = {};
    body = "";
}

HttpRequest::HttpRequest(std::string _method, std::string _url, std::string _version) {
    method = _method;
    url = _url;
    version = _version;
    headers = {};
    body = "";
}

std::string HttpRequest::serialize() {
    std::stringstream ss;
    ss << method.length() << "$" << method;
    ss << url.length() << "$" << url;
    ss << version.length() << "$" << version;
    
    ss << headers.size() << "$";
    for (const auto& pair : headers) {
        ss << pair.first.length() << "$" << pair.first;
        ss << pair.second.length() << "$" << pair.second;
    }

    ss << body.length() << "$" << body;
    return ss.str();
}

 HttpRequest HttpRequest::deserialize(const std::string& data) {
    HttpRequest req;
    std::string result = "";
    int newStart = extractFieldFrom(0, data, result);
    req.method = result;
    newStart = extractFieldFrom(newStart, data, result);
    req.url = result;
    newStart = extractFieldFrom(newStart, data, result);
    req.version = result;

    std::map<std::string, std::string> headers = {};
    newStart = extractHeaders(newStart, data, headers);
    req.headers = headers;

    newStart = extractFieldFrom(newStart, data, result);
    req.body = result;
    return req;
}

std::string HttpRequest::stringify() {
    std::stringstream ss;
    ss << method << " " << url << " " << version << "\n";
    for (const auto& pair : headers) {
        ss << pair.first << ": " << pair.second << "\n";
    }
    if (body.length() > 0) {
        ss << body << "\n";
    }
    return ss.str();
}
