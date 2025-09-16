
#ifndef TCP_IMPLEMENTATION_RESPONSEPROCESSOR_H
#define TCP_IMPLEMENTATION_RESPONSEPROCESSOR_H

#include "BodyParserFactory.h"
#include <unordered_map>
#include <string>



class ResponseProcessor {

    private:
        BodyParserFactory factory {};

    public:
        std::unordered_map<std::string, std::string> parse_body(
            const std::string& body_str,
            const std::string& content_type
        );
};






#endif