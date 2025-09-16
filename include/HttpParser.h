#ifndef TCP_IMPLEMENTATION_HTTPPARSER_H
#define TCP_IMPLEMENTATION_HTTPPARSER_H

#include <unordered_map>
#include <string>

class HttpParser {

    public:
        HttpParser() = default;
        std::unordered_map<std::string, std::string>& parse_headers(const std::string& raw_headers);

};











#endif //TCP_IMPLEMENTATION_HTTPPARSER_H