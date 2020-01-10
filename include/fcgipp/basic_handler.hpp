
#ifndef _HEADER_FILE_basic_handler_20200103163230_
#define _HEADER_FILE_basic_handler_20200103163230_

#include "request.hpp"
#include <memory>

namespace fcgipp {
    class BasicHandler {
    public:
        BasicHandler() = default;
        ~BasicHandler() = default;

        virtual void handle(std::shared_ptr<FcgiRequest> request) = 0;
    };

    template<typename Derived_t, typename... Arg_t>
    std::shared_ptr<BasicHandler> make_handler(Arg_t... args) {
        return std::make_shared<Derived_t>(args...);
    }
};

#endif