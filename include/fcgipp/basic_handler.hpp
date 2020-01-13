
#ifndef _HEADER_FILE_basic_handler_20200103163230_
#define _HEADER_FILE_basic_handler_20200103163230_

#include "req_res.hpp"
#include "http_method.hpp"
#include <memory>

namespace fcgipp {

    struct BasicHandler {
        ~BasicHandler() = default;
        virtual void handle(std::shared_ptr<FcgiReqRes> request) = 0;
    };

};

#endif