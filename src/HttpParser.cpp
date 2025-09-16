#include "../include/HttpParser.h"

#include <unordered_map>
#include <string>
#include <vector>
#include <array>


std::unordered_map<std::string, std::string>& parse_headers(const std::string& raw_request) {

    std::string header {};
    size_t i {0};
    std::vector<std::string> parsed_vec {};
    std::unordered_map<std::string, std::string> headers_map {};


    while (i < raw_request.length()) {
        if (raw_request[i] != '\n' && raw_request[i] != '\r') {
            header.push_back(raw_request[i]);
        } else if (raw_request[i] == '\r') {
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


std::unordered_map<std::string, std::string>& HttpParser::parse_body(const std::string& body_str) {
    
}
