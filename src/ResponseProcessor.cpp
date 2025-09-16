#include "../include/ResponseProcessor.h"





std::unordered_map<std::string, std::string> ResponseProcessor::parse_body(
    const std::string& body_str,
    const std::string& content_type
) {

    IBodyParser* parser = factory.get_parser(content_type);


    if (parser == nullptr) {
        perror("Parser is of null type.");
    }

    std::unordered_map<std::string, std::string> body_map = parser->parse(body_str);

    return body_map;
}