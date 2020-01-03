
#ifndef _HEADER_FILE_basic_handler_20200103163230_
#define _HEADER_FILE_basic_handler_20200103163230_

#include "request.hpp"
#include <memory>

class BasicHandler {
public:
    using Ptr_Type = std::shared_ptr<BasicHandler>;
    using Request_Ptr_Type = std::shared_ptr<FcgiRequest>;

    virtual void handle(Request_Ptr_Type request) = 0;
};

template<typename Derived_t>
BasicHandler::Ptr_Type make_handler() {
    return std::make_shared<Derived_t>();
}

#endif