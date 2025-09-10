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
#include "ThreadManager.h"

const char* PORT = "6511";
const int MAX_THREADS = 8;
const int BUFFER_SIZE = 8192;


int create_tcp_socket() {
    struct addrinfo hints{};
    struct addrinfo* res{};
    struct addrinfo* p{};
    int sockfd{};

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;


    int status = getaddrinfo(NULL, PORT, &hints, &res);

    if (status == -1) {
        perror("getaddrinfo failure");
        exit(EXIT_FAILURE);
    }

    for (p = res; p != nullptr; p = p->ai_next) {
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);

        if (sockfd == -1) {
            perror("creating socket error");
            continue;
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

    return sockfd;
}

std::unordered_map<std::string, std::string> parse_headers(const std::string& raw_headers) {
    /*   TODO:
     *      - this is mad spaghetti (its okay for now since its a prototype, since will be moving to OOP design after the prototype)
     *          - cleanup conditional statement flow by maybe converting to a switch statement flow instead, it will look way better.
     *          - change things to a const type where applicable, will be faster.
     *
     *
     *
     */
    std::string header {};
    size_t i {0};
    std::vector<std::string> parsed_vec {};
    std::unordered_map<std::string, std::string> headers_map {};

    /* =================================================================
     * Below we split up the request string into n strings where
     * first line is the start line followed by a bunch of request
     * header pairs.
     * =================================================================
     */

    while (i < raw_headers.length()) {
        if (raw_headers[i] != '\n' && raw_headers[i] != '\r') {
            header.push_back(raw_headers[i]);
        } else if (raw_headers[i] == '\r') {
            header.push_back(' ');
            parsed_vec.push_back(header);
            header.clear();
            ++i;
        }
        ++i;
    }


    for (auto vec : parsed_vec) {
        std::cout << vec << "\n";
    }
    /* =================================================================
     * Below we tokenise the request line into key-value pairs.
     *
     * E.g.,
     *
     * key : value
     * HTTP_METHOD: POST
     * HTTP_URL: /submit-form
     * HTTP_VERSION: HTTP/1.1
     * Content-Type: application/x-www-form-urlencoded
     * Content-Length: 27
     *  ...
     *
     * =================================================================
     */

    for (size_t i{0}; i < parsed_vec.size(); i++) {
        std::string header_token {};
        if (i == 0) {
            size_t j{0};
            int count{0};
            while (j < parsed_vec[0].length()) {
                if (parsed_vec[0][j] != ' ') {
                    header_token.push_back(parsed_vec[0][j]);
                } else {
                    if (count == 0) {
                        headers_map.insert({"HTTP_METHOD", header_token});
                    } else if (count == 1) {
                        headers_map.insert({"HTTP_URL", header_token});
                    } else if (count == 2) {
                        headers_map.insert({"HTTP_VERSION", header_token});
                    }
                    std::cout << count << "\n";
                    ++count;
                    header_token.clear();
                }
                ++j;
            }
        } else {
            size_t j{0};
            int count{0};
            std::array<std::string, 2> header_token_pair{};
            while (j < parsed_vec[i].length()) {
                if (parsed_vec[i][j] == ' ') {
                    if (count == 0) {
                        header_token_pair[0] = header_token;
                        ++count;
                    } else {
                        header_token_pair[1] = header_token;
                        headers_map.insert({header_token_pair[0], header_token_pair[1]});
                        count = 0;
                    }
                    header_token.clear();
                } else if (parsed_vec[i][j] == '\r' || parsed_vec[i][j] == '\n') {
                    ;
                } else {
                    header_token.push_back(parsed_vec[i][j]);
                }
                ++j;
            }
        }
    }

    return headers_map;
}


std::string readHttpRequest(const int client_sockfd) {

    std::vector<char> buffer(BUFFER_SIZE);
    std::string request {};
    ssize_t bytes_read {};



    while (true) {
        bytes_read = recv(client_sockfd, buffer.data(), buffer.size(), 0);

        if (bytes_read < 0) {
            perror("failed to receive bytes from client socket");
            break;
        } else if (bytes_read == 0) {
            // we finished reading the request.
            close(client_sockfd);
            break;
        }

        request.append(buffer.data(), bytes_read);

        size_t header_end_pos = request.find("\r\n\r\n");

        if (header_end_pos != std::string::npos) {
        }
    }
}

int accept_client_connection(const int sockfd) {

    struct sockaddr_storage client_addr {};
    socklen_t addr_size = sizeof(client_addr);
    std::cout << "waiting to accept... \n";
    int new_fd = accept(sockfd, reinterpret_cast<struct sockaddr *>(&client_addr), &addr_size);
    if (new_fd == -1) {
        perror("error in creating accept new socket connection");
    }

    return new_fd;
}



void handle_client(const int client_sockfd, ThreadManager& thread_manager) {

    char buff[8192];
    ssize_t num_bytes{};

    while ((num_bytes = recv(client_sockfd, buff, sizeof(buff) - 1, 0)) > 0) {
        buff[num_bytes] = '\0';

        if (strcmp(&buff[0], "quit\r\n") == 0 || strcmp(&buff[0], "quit\n") == 0) {
            std::cout<< "Client disconnected" << "\n";
            close(client_sockfd);
            break;
        }

        if (num_bytes > 0) {
            send(client_sockfd, buff, num_bytes, 0);
        }
    }
    thread_manager.decrement();
    close(client_sockfd);
}


int main() {
    ThreadManager thread_manager = ThreadManager();
    std::vector<std::thread> thread_pool {};
    int sockfd {create_tcp_socket()};
    std::cout << "waiting for a client to connect..." << "\n";


    while (true) {
        int client_sockfd {accept_client_connection(sockfd)};
        std::cout << "Accepted client on socket fd: " << client_sockfd << "\n";
        if (thread_manager.get_count() < MAX_THREADS) {
            thread_manager.increment();
            thread_pool.emplace_back(handle_client, client_sockfd, std::ref(thread_manager));

        }
    }
}

