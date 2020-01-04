
#ifndef _HEADER_FILE_uri_dispatcher_20200103234120_
#define _HEADER_FILE_uri_dispatcher_20200103234120_

#include "basic_handler.hpp"
#include "handlers.hpp"
#include "fcgiapp.h"
#include <map>
#include <memory>
#include <string>

template<typename Async_t, typename Authenticator_t>
class UriDispatcher {
public:

    UriDispatcher(Async_t &aio, Authenticator_t &auth)
        : m_async_scheduler(aio)
        , m_authenticator(auth)
        {}

    void dispatch(BasicHandler::Request_Ptr_Type req_ptr) {
        BasicHandler::Ptr_Type current_handler;

        if (m_authenticator.is_valid(req_ptr)) {
            auto uri = FCGX_GetParam("PATH_INFO", req_ptr->envp());

            if (uri) {
                auto key = std::string(uri);
                auto it = m_dispatch_matrix.find(key);
                if ( it != m_dispatch_matrix.end() ) {
                    current_handler = it->second;
                } else {
                    current_handler = m_handler_404;
                }
            } else { // PATH_INFO is not here we are aiming for /
                auto key = std::string("/");
                auto it = m_dispatch_matrix.find(key);
                if ( it != m_dispatch_matrix.end() ) {
                    current_handler = it->second;
                } else {
                    current_handler = m_handler_404;
                }
            }
        } else {
            current_handler = m_handler_401;
        }

        m_async_scheduler.post([req_ptr, current_handler]{
            current_handler->handle(req_ptr);
        });
    }

    void add_endpoint(std::string uri, BasicHandler::Ptr_Type req) {
        m_dispatch_matrix[uri] = req;
    }

private:
    std::map<std::string, BasicHandler::Ptr_Type> m_dispatch_matrix;

    BasicHandler::Ptr_Type m_handler_404 = make_handler<DefaultNotFoundHandler>();
    BasicHandler::Ptr_Type m_handler_401 = make_handler<DefaultUnauthorizedHandler>();

    Async_t &m_async_scheduler;
    Authenticator_t &m_authenticator;
};

template<typename Async_t, typename Authenticator_t>
UriDispatcher<Async_t, Authenticator_t> make_dispatcher(Async_t &async, Authenticator_t &auth) {
    return UriDispatcher<Async_t, Authenticator_t>(async, auth);
}

#endif