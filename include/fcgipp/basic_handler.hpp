
#ifndef _HEADER_FILE_basic_handler_20200103163230_
#define _HEADER_FILE_basic_handler_20200103163230_

#include "req_res.hpp"
#include "http_method.hpp"
#include <memory>

namespace fcgipp {

    class BasicHandler {
        HttpMethod m_method;
    public:
        BasicHandler(HttpMethod m = HttpMethod::Get) : m_method(m) {}
        ~BasicHandler() = default;

        virtual void handle(std::shared_ptr<FcgiReqRes> request) = 0;

        HttpMethod const& get_method() {
            return m_method;
        }
    };

};

#endif