
#ifndef _HEADER_FILE_basic_dispatcher_20200115194648_
#define _HEADER_FILE_basic_dispatcher_20200115194648_

#include <memory>
#include "basic_handler.hpp"
#include "basic_request_response.hpp"

namespace fcgipp {

    struct BasicDispatcher {
        virtual ~BasicDispatcher() = default;
        virtual void dispatch(std::shared_ptr<BasicRequestResponse> req_ptr) = 0;
    };

};

#endif