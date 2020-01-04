
#ifndef _HEADER_FILE_uri_dispatcher_20200103234120_
#define _HEADER_FILE_uri_dispatcher_20200103234120_

#include "basic_handler.hpp"
#include "handlers.hpp"
#include "fcgiapp.h"
#include <map>
#include <memory>
#include <string>



template<typename Authenticator_t>
class UriDispatcher {
public:

    UriDispatcher(Authenticator_t &auth) : m_authenticator(auth) {}

    void dispatch(BasicHandler::Request_Ptr_Type req_ptr) {
        if (m_authenticator.is_valid(req_ptr)) {

            auto uri = FCGX_GetParam("REQUEST_URI", req_ptr->envp());

            if (uri) {
                auto key = std::string(uri);
                auto it = m_dispatch_matrix.find(key);
                if ( it != m_dispatch_matrix.end() ) {
                    BasicHandler::Ptr_Type handler_ptr = it->second;
                    handler_ptr->handle(req_ptr);
                } else {
                    m_handler_404->handle(req_ptr);
                }
            } else {
                m_handler_404->handle(req_ptr);
            }
        } else {
            m_handler_401->handle(req_ptr);
        }
    }

    void add_handler(std::string uri, BasicHandler::Ptr_Type req) {
        m_dispatch_matrix[uri] = req;
    }

private:
    std::map<std::string, BasicHandler::Ptr_Type> m_dispatch_matrix;

    BasicHandler::Ptr_Type m_handler_404 = make_handler<DefaultNotFoundHandler>();
    BasicHandler::Ptr_Type m_handler_401 = make_handler<DefaultUnauthorizedHandler>();

    Authenticator_t &m_authenticator;
};

#endif