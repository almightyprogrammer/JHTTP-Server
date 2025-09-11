//
// Created by humblefarmer on 04/09/2025.
//

#ifndef TCP_IMPLEMENTATION_REQUEST_H
#define TCP_IMPLEMENTATION_REQUEST_H
#include "../include/HttpMethod.h"
#include <string>
#include <unordered_map>


class Request {
private:
    HttpMethod method;
    std::string url;
    std::unordered_map<std::string, std::string> headers;
    std::unordered_map<std::string, std::string> body;
public:
    Request();
    Request(HttpMethod method,
            std::string url,
            std::unordered_map<std::string,
            std::string> headers,
            std::unordered_map<std::string, std::string> body);

    [[nodiscard]] const HttpMethod& get_method() const;
    [[nodiscard]] const std::string& get_url() const;
    [[nodiscard]] const std::unordered_map<std::string, std::string>& get_headers() const;
    [[nodiscard]] const std::unordered_map<std::string, std::string>& get_body () const;
};


#endif //TCP_IMPLEMENTATION_REQUEST_H