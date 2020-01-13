
#ifndef _HEADER_FILE_handler_response_20200110190205_
#define _HEADER_FILE_handler_response_20200110190205_

#include <ostream>
#include <string>

namespace fcgipp {
    struct BasicResponse {
        virtual ~BasicResponse() = default;
        virtual std::string render() = 0;
    };
};

#endif