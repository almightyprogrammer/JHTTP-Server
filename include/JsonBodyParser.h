
#ifndef TCP_IMPLEMENTATION_JSONBODYPARSER_H
#define TCP_IMPLEMENTATION_JSONBODYPARSER_H

#include "IBodyParser.h"

class JsonBodyParser : IBodyParser {

    public:
        bool supports(const std::string& content_type);
        std::unordered_map<std::string, std::string>& parse(const std::string& body_str);

};




#endif