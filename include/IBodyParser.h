
#ifndef TCP_IMPLEMENTATION_IBODYPARSER_H
#define TCP_IMPLEMENTATION_IBODYPARSER_H

#include <unordered_map>
#include <string>

class IBodyParser {
    public:
        virtual ~IBodyParser() = default;
        virtual std::unordered_map<std::string, std::string> parse(const std::string& body_str) = 0; 
        virtual bool supports(const std::string& content_type) = 0;
    };


#endif