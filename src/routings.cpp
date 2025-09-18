#include "../include/routings.h"

std::tuple<std::string, std::string> handle_greetings(const Request& req) {
    std::string content_type = "text/html";
    std::string body = "<html><head><h2>Greetings from Juwon Han</h2></head></html>";

    return std::make_tuple(content_type, body);
}