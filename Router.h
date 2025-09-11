//
// Created by humblefarmer on 11/09/2025.
//

#include <unordered_map>
#include <functional>
#include "Request.h"


#ifndef TCP_IMPLEMENTATION_ROUTER_H
#define TCP_IMPLEMENTATION_ROUTER_H


class Router {

public:
    using Handler = std::function<void(const Request&)>;
    Router();

private:
    std::unordered_map<std::string, Handler> routes {};

public:
    void add_route(const std::string url, Handler handler);
    void handle(const Request& req) const;
};


#endif //TCP_IMPLEMENTATION_ROUTER_H