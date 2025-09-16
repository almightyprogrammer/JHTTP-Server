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
#include "routings.cpp"
#include "../include/ThreadManager.h"
#include "../include/Request.h"
#include "../include/Router.h"
#include "../include/HttpServer.h"
#include "../include/routings.h"

const int MAX_THREADS = 8;




int main() {
    
    HttpServer server{};
    Router& router = server.get_router();
    ThreadManager& thread_manager = server.get_thread_manager();
    std::vector<std::thread> thread_pool {};
    int sockfd = server.get_sockfd();
    router.add_route("/greetings", [](const Request&) {
    return handle_greetings();
    });
    std::cout << "waiting for a client to connect..." << "\n";


    while (true) {
        int client_sockfd {server.accept_client_connection()};
        std::cout << "Accepted client on socket fd: " << client_sockfd << "\n";
        if (thread_manager.get_count() < MAX_THREADS) {
            thread_manager.increment();
            thread_pool.emplace_back([&server, client_sockfd]() {
                server.handle_client(client_sockfd);
            });
        }
    }
}