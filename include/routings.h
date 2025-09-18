#ifndef TCP_IMPLEMENTATION_ROUTINGS_H
#define TCP_IMPLEMENTATION_ROUTINGS_H


#include <string>
#include "../include/Request.h"
#include <unordered_map>
#include <tuple>

std::tuple<std::string, std::string> handle_greetings(const Request& req);

#endif