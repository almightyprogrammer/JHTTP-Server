//
// Created by humblefarmer on 11/09/2025.
//

#include "Router.h"



Router::Router() : routes() {}

void Router::add_route(const std::string url, Handler handler) {
    routes[url] = std::move(handler);
}

void Router::handle(const Request& req) const {
    auto it = routes.find(req.get_url());

    if (it != routes.end()) {
        it->second(req);
    } else {
        throw std::logic_error("Such an endpoint does not exist or is yet to be registered in the router.");
    }
}