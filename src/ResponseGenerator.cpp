#include "../include/ResponseGenerator.h"
#include <iostream>
#include "../include/Response.h"

std::string ResponseGenerator::generate_new_response(Response res) {
    std::unordered_map<std::string, std::string> headers_map = res.get_headers();
    std::string body = res.get_body();
    std::string status_code = res.get_status_code();
    std::string content_type = res.get_content_type();
    std::string code_description = get_code_description(status_code);


    std::string response {};

    
    response += "HTTP/1.0 " + status_code + " " + code_description + "\r\n";
    response += "Server: JHTTP Server (Juwon Han Text Transfer Protocol)\r\n";
    response += "Content-Type: " + content_type + "\r\n";
    response += "Content-Length: " + std::to_string(body.size()) + "\r\n";
    for (const auto& [key, value] : headers_map) {
        if (key == "Content-Type" || key == "Content-Length") continue;
        response += key + ": " + value + "\r\n";
    }

    response += "\r\n";   
    response += body;

    return response;
}


std::string ResponseGenerator::get_code_description(const std::string& s_code) {

    if (s_code == "200") {
        return "OK";
    } else if (s_code == "404") {
        return "Not Found";
    } else if (s_code == "500") {
        return "Internal Server Error";
    } else {
        perror("status code -> code description logic error");
        return "";
    }
}