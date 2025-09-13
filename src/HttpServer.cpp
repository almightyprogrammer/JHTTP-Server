//
// Created by humblefarmer on 12/09/2025.
//


#include "../include/HttpServer.h"




HttpServer::HttpServer()  {
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    int yes = 1;
    int status = getaddrinfo(nullptr, PORT, &hints, &res);

    if (status != 0) {
        perror("getaddrinfo failure");
        exit(EXIT_FAILURE);
    }

    for (p = res; p != nullptr; p = p->ai_next) {
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

        if (sockfd < 0) {
            perror("creating socket error");
            continue;
        }
        
        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(yes)) == -1) {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }               
        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("bind failed");
            continue;
        }
        break;
    }

    if (p == nullptr) {
        std::cerr << "server: failed to bind\n";
        freeaddrinfo(res);
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(res);

    if (listen(sockfd, 10) == -1) {
        perror("listening to socket failed");
        close(sockfd);
        exit(EXIT_FAILURE);
    }
}

HttpServer::~HttpServer()  {
    if (sockfd != -1) {
        close(sockfd);
    }
}

void HttpServer::handle_client(const int client_sockfd) {

    



}


int HttpServer::accept_client_connection() {
    struct sockaddr_storage client_addr{};
    socklen_t addr_size = sizeof(client_addr);
    std::cout << "waiting to accept...\n";
    int new_client_fd = accept(sockfd, reinterpret_cast<struct sockaddr *>(&client_addr), &addr_size);

    if (new_client_fd == -1) {
        perror("error in accepting new socket connection...");
    }

    return new_client_fd;
}

