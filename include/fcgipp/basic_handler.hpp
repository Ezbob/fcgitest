
#ifndef _HEADER_FILE_basic_handler_20200103163230_
#define _HEADER_FILE_basic_handler_20200103163230_

#include "req_res.hpp"
#include "http_method.hpp"
#include <memory>

namespace fcgipp {

    class BasicHandler {
        HttpMethod m_method;
    public:
        BasicHandler(HttpMethod m = HttpMethod::GET) : m_method(m) {}
        ~BasicHandler() = default;

        virtual void handle(std::shared_ptr<FcgiReqRes> request) = 0;

        HttpMethod const& get_method() {
            return m_method;
        }
    };

    template<typename Derived_t, typename... Arg_t>
    std::shared_ptr<BasicHandler> make_handler(Arg_t... args) {
        return std::make_shared<Derived_t>(args...);
    }

};

#endif