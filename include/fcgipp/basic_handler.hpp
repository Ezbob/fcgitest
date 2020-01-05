
#ifndef _HEADER_FILE_basic_handler_20200103163230_
#define _HEADER_FILE_basic_handler_20200103163230_

#include "request.hpp"
#include <memory>

namespace fcgipp {
    class BasicHandler {
    public:
        BasicHandler() = default;
        ~BasicHandler() = default;

        using Ptr_Type = std::shared_ptr<BasicHandler>;
        using Request_Ptr_Type = std::shared_ptr<FcgiRequest>;

        virtual void handle(Request_Ptr_Type request) = 0;
    };

    template<typename Derived_t, typename... Arg_t>
    BasicHandler::Ptr_Type make_handler(Arg_t... args) {
        return std::make_shared<Derived_t>(args...);
    }
};

#endif