//
// Created by humblefarmer on 12/09/2025.
//

#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <thread>
#include <atomic>
#include <vector>
#include <array>
#include <unordered_map>
#include <stdexcept>
#include "../include/ThreadManager.h"
#include "../include/Request.h"
#include "../include/Router.h"






#ifndef TCP_IMPLEMENTATION_HTTPSERVER_H
#define TCP_IMPLEMENTATION_HTTPSERVER_H

#endif //TCP_IMPLEMENTATION_HTTPSERVER_H



class HttpServer {

private:
    struct addrinfo hints{};
    struct addrinfo* res{};
    struct addrinfo* p{};
    int sockfd{};
    const char* PORT{"6511"};
    Router router{};

public:
    HttpServer() : hints(), res(), p(), sockfd() {};
    ~HttpServer() {};
    void handle_client(const int client_sockfd);
    int accept_client_connection();

};