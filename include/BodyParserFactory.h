

#ifndef TCP_IMPLEMENTATION_BODYPARSERFACTORY_H
#define TCP_IMPLEMENTATION_BODYPARSERFACTORY_H

#include <vector>
#include "IBodyParser.h"
#include <memory>
#include "JsonBodyParser.h"
#include "XmlBodyParser.h"

class BodyParserFactory {

    private:
        std::vector<std::unique_ptr<IBodyParser>> parsers {};

    public:
        BodyParserFactory() {
            parsers.emplace_back(std::make_unique<JsonBodyParser>());
            parsers.emplace_back(std::make_unique<XmlBodyParser>());
        }
        
        IBodyParser* get_parser(const std::string& content_type);
};





#endif 