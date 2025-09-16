

#ifndef TCP_IMPLEMENTATION_XMLBODYPARSER_H
#define TCP_IMPLEMENTATION_XMLBODYPARSER_H

#include <string>
#include <unordered_map>
#include "IBodyParser.h"

class XmlBodyParser : IBodyParser {

    public:
        bool supports(const std::string& content_type);
        std::unordered_map<std::string, std::string>& parse(const std::string& body_str);

}





#endif