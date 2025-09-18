#ifndef TCP_IMPLEMENTATION_RESPONSE_H
#define TCP_IMPLEMENTATION_RESPONSE_H

#include <unordered_map>
#include <string>


class Response {
    private:
        std::unordered_map<std::string, std::string> headers_map {};
        std::string body {};
        std::string status_code {};
        std::string content_type {};

    public:
        Response();
        Response(std::unordered_map<std::string, std::string> h_map, std::string b,
            std::string c_type, std::string s_code
        );
        std::unordered_map<std::string, std::string> get_headers();
        std::string get_body();
        std::string get_status_code();
        std::string get_content_type();

};


#endif