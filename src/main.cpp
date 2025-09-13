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

const char* PORT = "6511";
const int MAX_THREADS = 8;
const int BUFFER_SIZE = 8192;
Router router{};

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

    std::string header {};
    size_t i {0};
    std::vector<std::string> parsed_vec {};
    std::unordered_map<std::string, std::string> headers_map {};


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


std::unordered_map<std::string, std::string> parse_body(std::string request) {
    throw std::logic_error("Not implemented yet");
}


Request read_http_request(const int client_sockfd) {
    std::vector<char> buffer(BUFFER_SIZE);
    std::string request{};
    ssize_t bytes_read{};
    std::unordered_map<std::string, std::string> headers_map{};
    std::unordered_map<std::string, std::string> body_map{};
    int content_length = 0;
    size_t content_begin_idx = 0;
    Request http_request{};

    while (true) {
        bytes_read = recv(client_sockfd, buffer.data(), buffer.size(), 0);

        if (bytes_read < 0) {
            perror("failed to receive bytes from client socket");
            break;
        } else if (bytes_read == 0) {
            close(client_sockfd);
            break;
        }

        request.append(buffer.data(), bytes_read);

        size_t header_end_pos = request.find("\r\n\r\n");
        if (header_end_pos != std::string::npos && content_length == 0) {
            headers_map = parse_headers(request.substr(0, header_end_pos));

            auto it = headers_map.find("Content-Length");
            if (it != headers_map.end()) {
                content_length = std::stoi(it->second);
            }
            content_begin_idx = header_end_pos + 4; // \r\n\r\n length is 4
        }

        if (content_length > 0) {
            size_t body_received = request.size() - content_begin_idx;
            if (body_received >= static_cast<size_t>(content_length)) {
                break;
            }
        } else if (header_end_pos != std::string::npos) {
            break;
        }
    }

    HttpMethod method {};
    std::string url {headers_map.find("HTTP_URL")->second};


    for (const auto& header_pair : headers_map) {
        std::string method_string = header_pair.second;
        if (method_string == "GET") { method = HttpMethod::GET;}
        else if (method_string == "POST") {method = HttpMethod::POST;}
        else if (method_string == "PUT") {method = HttpMethod::PUT;}
        else if (method_string == "PATCH") {method = HttpMethod::PATCH;}
        else if (method_string == "DELETE") {method = HttpMethod::DELETE;}
        else if (method_string == "HEAD") {method = HttpMethod::HEAD;}
        else if (method_string == "OPTIONS") {method = HttpMethod::OPTIONS;}
        else if (method_string == "UNKNOWN") {method = HttpMethod::UNKNOWN;}
    }


    if (headers_map.find("HTTP_METHOD")->second == "POST" || headers_map.find("HTTP_METHOD")->second == "PUT" || headers_map.find("HTTP_METHOD")->second == "PATCH") {
        body_map = parse_body(request);
    }

    Request new_request(method, url, headers_map, body_map);

    return new_request;

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

std::string handle_response(const std::string& handler_output) {
    std::string response;

    if (handler_output == "___404_NOT_FOUND___") {
            response =
                "HTTP/1.1 404 Not Found\r\n"
                "Content-Type: text/plain\r\n"
                "Content-Length: 9\r\n"
                "Connection: close\r\n"
                "\r\n"
                "Not Found";
        } else {
            response =
                "HTTP/1.1 200 OK\r\n"
                "Content-Type: text/plain\r\n"
                "Content-Length: " + std::to_string(handler_output.size()) + "\r\n"
                "Connection: close\r\n"
                "\r\n" +
                handler_output;
        }
    return response;


}


void handle_client(const int client_sockfd, ThreadManager& thread_manager) {

    Request http_request = read_http_request(client_sockfd);
    std::string handler_output = router.handle(http_request);
    std::string response = handle_response(handler_output);
    ssize_t sent_bytes = send(client_sockfd, response.c_str(), response.size(), 0);

    if (sent_bytes < 0) {
        perror("sending failed");
    }
    thread_manager.decrement();
    close(client_sockfd);
}


int main() {
    ThreadManager thread_manager = ThreadManager();
    std::vector<std::thread> thread_pool {};
    int sockfd {create_tcp_socket()};
    router.add_route("/greetings", [](const Request&) {return "Hello";});
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