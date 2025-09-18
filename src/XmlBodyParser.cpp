#include "../include/XmlBodyParser.h"





bool XmlBodyParser::supports(const std::string& content_type) {
    
    return (content_type == "application/xml");

}

std::unordered_map<std::string, std::string> XmlBodyParser::parse(const std::string& body_str) {

}