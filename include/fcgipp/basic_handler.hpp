
#ifndef _HEADER_FILE_basic_handler_20200103163230_
#define _HEADER_FILE_basic_handler_20200103163230_

#include "basic_request_response.hpp"
#include <memory>

namespace fcgipp {

    struct BasicHandler {
        virtual ~BasicHandler() = default;
        virtual void handle(std::shared_ptr<BasicRequestResponse> request) = 0;
    };

};

#endif