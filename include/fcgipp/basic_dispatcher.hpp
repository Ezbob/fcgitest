
#ifndef _HEADER_FILE_basic_dispatcher_20200115194648_
#define _HEADER_FILE_basic_dispatcher_20200115194648_

#include <memory>
#include "basic_handler.hpp"
#include "fcgi_req_res.hpp"

namespace fcgipp {

    struct BasicDispatcher {
        virtual ~BasicDispatcher() = default;
        virtual std::shared_ptr<BasicHandler> dispatch(std::shared_ptr<FcgiReqRes> req_ptr) = 0;
    };

};

#endif