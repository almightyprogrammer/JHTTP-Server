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
    const char* PORT = "6511";


public:
    HttpServer() : hints(), res(), p(), sockfd() {
        memset(&hints, 0, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;

        int status = getaddrinfo(nullptr, PORT, &hints, &res);

        if (status < 0) {
            perror("getaddrinfo failure");
            exit(EXIT_FAILURE);
        }

        for (p = res; p != nullptr; p = p->ai_next) {
            sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

            if (sockfd < 0) {
                perror("creating socket error");
                continue;
            }


        }
    }





};