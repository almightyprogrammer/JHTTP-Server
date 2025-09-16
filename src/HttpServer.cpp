//
// Created by humblefarmer on 12/09/2025.
//


#include "../include/HttpServer.h"
#include "../include/ResponseProcessor.h"



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



Request HttpServer::read_http_request(const int client_sockfd)  {
    std::vector<char> buffer(BUFFER_SIZE);
    std::string request{};
    ssize_t bytes_read{};
    std::unordered_map<std::string, std::string> headers_map{};
    std::unordered_map<std::string, std::string> body_map{};
    int content_length = 0;
    size_t content_begin_idx = 0;
    Request http_request{};
    size_t body_received{};

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
            headers_map = parser.parse_headers(request.substr(0, header_end_pos));

            auto it = headers_map.find("Content-Length");
            if (it != headers_map.end()) {
                content_length = std::stoi(it->second);
            }
            content_begin_idx = header_end_pos + 4; // \r\n\r\n length is 4
        }

        if (content_length > 0) {
            body_received = request.size() - content_begin_idx;
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
        auto it = headers_map.find("Content-Type");
        
        body_map = response_processor.parse_body(request.substr(body_received, std::string::npos), it->second);
    }

    Request new_request(method, url, headers_map, body_map);



    return new_request;

}
 





void HttpServer::handle_client(const int client_sockfd) {

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
 


