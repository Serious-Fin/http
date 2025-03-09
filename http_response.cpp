#include "http_response.h"
#include <sstream>
#include <string>

int HttpResponse::extractFieldFrom(int start, const std::string& data, std::string &result) {
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

int HttpResponse::extractHeaders(int start, const std::string& data, std::map<std::string, std::string>& headers) {
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

HttpResponse::HttpResponse() {
    version = "";
    status = "";
    statusMsg = "";
    headers = {};
    body = "";
}

HttpResponse::HttpResponse(std::string _version, std::string _status, std::string _statusMsg) {
    version = _version;
    status = _status;
    statusMsg = _statusMsg;
    headers = {};
    body = "";
}

std::string HttpResponse::serialize() {
    std::stringstream ss;
    ss << version.length() << "$" << version;
    ss << status.length() << "$" << status;
    ss << statusMsg.length() << "$" << statusMsg;
    
    ss << headers.size() << "$";
    for (const auto& pair : headers) {
        ss << pair.first.length() << "$" << pair.first;
        ss << pair.second.length() << "$" << pair.second;
    }

    ss << body.length() << "$" << body;
    return ss.str();
}

 HttpResponse HttpResponse::deserialize(const std::string& data) {
    HttpResponse req;
    std::string result = "";
    int newStart = extractFieldFrom(0, data, result);
    req.version = result;
    newStart = extractFieldFrom(newStart, data, result);
    req.status = result;
    newStart = extractFieldFrom(newStart, data, result);
    req.statusMsg = result;

    std::map<std::string, std::string> headers = {};
    newStart = extractHeaders(newStart, data, headers);
    req.headers = headers;

    newStart = extractFieldFrom(newStart, data, result);
    req.body = result;
    return req;
}

std::string HttpResponse ::stringify() {
    std::stringstream ss;
    ss << version << " " << status << " " << statusMsg << "\n";
    for (const auto& pair : headers) {
        ss << pair.first << ": " << pair.second << "\n";
    }
    if (body.length() > 0) {
        ss << body << "\n";
    }
    return ss.str();
}
