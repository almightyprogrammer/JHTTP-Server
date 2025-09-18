//
// Created by humblefarmer on 11/09/2025.
//

#include "../include/Router.h"
#include "../include/Response.h"
#include <string>
#include <stdexcept>


Router::Router() : routes() {}

void Router::add_route(const std::string url, Handler handler) {
    routes[url] = std::move(handler);
}

Response Router::handle(const Request& req) {
    
    std::unordered_map<std::string, std::string> headers{};
    auto it = routes.find(req.get_url());
    std::string response_body {};
    std::string status_code {};
    std::string content_type {};
    if (it != routes.end()) {
        status_code = "200";
        auto [content_type, body] = it->second(req);
        response_body = body;
        
    } else {
        status_code = "404";
        content_type = "text/plain";
        response_body = "404 ERROR";
    }

    Response response(headers, response_body, content_type, status_code);

    return response;
}