#ifndef TCP_IMPLEMENTATION_RESPONSEGENERATOR_H
#define TCP_IMPLEMENTATION_RESPONSEGENERATOR_H

#include <string>
#include "Response.h"

class ResponseGenerator {

    public:
        std::string generate_new_response(Response res);
        std::string get_code_description(const std::string& s_code);
};



#endif