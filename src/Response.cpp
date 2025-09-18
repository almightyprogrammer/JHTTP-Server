#include "../include/Response.h"

Response::Response() = default;

Response::Response(std::unordered_map<std::string, std::string> h_map, std::string b, std::string c_type, std::string s_code)
    : headers_map(h_map), body(b), content_type(c_type), status_code(s_code) {}

std::unordered_map<std::string, std::string> Response::get_headers() {
    return headers_map;
}

std::string Response::get_body() {
    return body;
}

std::string Response::get_content_type() {
    return content_type;
}

std::string Response::get_status_code() {
    return status_code;
}


