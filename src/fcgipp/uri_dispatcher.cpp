
#include "fcgipp/uri_dispatcher.hpp"

void UriDispatcher::dispatch(BasicHandler::Request_Ptr_Type req_ptr) {
    auto uri = FCGX_GetParam("REQUEST_URI", req_ptr->envp());

    if (uri) {
        auto key = std::string(uri);
        auto it = m_dispatch_matrix.find(key);
        if ( it != m_dispatch_matrix.end() ) {
            BasicHandler::Ptr_Type handler_ptr = it->second;

            m_io_ctx.post([handler_ptr, req_ptr] {
                handler_ptr->handle(req_ptr);
            });
        } else {

            m_io_ctx.post([this, req_ptr] {
                m_not_found_handler_ptr->handle(req_ptr);
            });
        }
    } else {
        m_io_ctx.post([this, req_ptr] {
            m_not_found_handler_ptr->handle(req_ptr);
        });
    }
}

void UriDispatcher::add_handler(std::string uri, BasicHandler::Ptr_Type req) {
    m_dispatch_matrix[uri] = req;
}
