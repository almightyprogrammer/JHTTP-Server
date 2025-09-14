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
    const int BUFFER_SIZE{8192};
    ThreadManager thread_manager = ThreadManager();
    

public:
    HttpServer();
    ~HttpServer();
    void handle_client(const int client_sockfd);
    int accept_client_connection();
    Request read_http_request(const int client_sockfd);
    std::unordered_map<std::string, std::string> parse_headers(const std::string& raw_headers);
    std::unordered_map<std::string, std::string> parse_body(std::string request);
    std::string handle_response(const std::string& handler_output);
    int get_sockfd();
    Router& get_router();
    ThreadManager& get_thread_manager();

};