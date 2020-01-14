
#include "fcgipp/dispatcher.hpp"

using namespace fcgipp;

void DefaultDispatcher::dispatch(std::shared_ptr<FcgiReqRes> req_ptr) {
    std::shared_ptr<BasicHandler> current_handler;

    if ( m_authenticator.is_valid(req_ptr) ) {
        auto raw_method = FCGX_GetParam("REQUEST_METHOD", req_ptr->envp());

        if (!raw_method) {
            return; // triggers a 500 error in lighttpd
        }

        std::string key;
        auto uri = FCGX_GetParam("PATH_INFO", req_ptr->envp());

        if (uri) {
            key = uri;
            add_end_slash(key);
        } else { // PATH_INFO is not here we are aiming for /
            key = "/";
        }

        auto it = m_dispatch_matrix.find(key);
        if ( it != m_dispatch_matrix.end() ) {
            HttpMethod actual_method = string_to_httpmethod(raw_method);

            if (actual_method == HttpMethod::Not_a_method) return;

            DefaultDispatcher::HandlerEntry &entry = it->second;

            if (entry.is_accepted_method(actual_method)) {
                current_handler = entry.handler;
            } else {
                current_handler = m_handler_404;
            }
        } else {
            current_handler = m_handler_404;
        }
    } else {
        current_handler = m_handler_401;
    }

    m_async_scheduler.post([req_ptr, current_handler]{
        current_handler->handle(req_ptr);
    });
}

void DefaultDispatcher::add_endpoint(std::string uri, HttpMethod meth, std::shared_ptr<BasicHandler> handler) {
    add_end_slash(uri);
    auto endpoint_it = m_dispatch_matrix.find(uri);
    if (endpoint_it == m_dispatch_matrix.end()) {
        DefaultDispatcher::HandlerEntry new_entry = {
            { meth },
            handler
        };
        m_dispatch_matrix.emplace(std::make_pair(uri, new_entry));
    } else {
        DefaultDispatcher::HandlerEntry &entry = endpoint_it->second;

        if ( !entry.is_accepted_method(meth) ) {
            entry.add_accepted_method(meth);
        } else {
            throw std::invalid_argument("Endpoint with method already exist");
        }
    }
}