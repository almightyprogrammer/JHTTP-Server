#include "../include/JsonBodyParser.h"



std::unordered_map<std::string, std::string> JsonBodyParser::parse(const std::string& body_str) {

    


};


bool JsonBodyParser::supports(const std::string& content_type) {

    return (content_type == "application/json");

}



