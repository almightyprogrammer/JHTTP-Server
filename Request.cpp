//
// Created by humblefarmer on 04/09/2025.
//

#include "Request.h"
#include "HttpMethod.h"


Request::Request(HttpMethod method,
        std::string url,
        std::unordered_map<std::string,std::string> headers,
        std::string body)
    : method(method),
      url(std::move(url)),
      headers(std::move(headers)),
      body(std::move(body)) {}

const HttpMethod& Request::get_method() const {
        return method;
}

const std::string& Request::get_url() const {
    return url;
};
const std::unordered_map<std::string, std::string>& Request::get_headers() const {
    return headers;
};

const std::string& Request::get_body() const {
    return body;
};