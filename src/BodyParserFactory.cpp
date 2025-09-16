#include "../include/BodyParserFactory.h"






IBodyParser* BodyParserFactory::get_parser(const std::string& content_type) {

    for (auto& parser : parsers) {
        if (parser->supports(content_type)) {
            return parser.get();
        }
    }

    return nullptr;
}

