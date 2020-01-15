
#include "fcgipp/dispatcher.hpp"

using namespace fcgipp;

std::shared_ptr<BasicHandler> DefaultDispatcher::dispatch(std::shared_ptr<FcgiReqRes> req_ptr) {
    std::shared_ptr<BasicHandler> current_handler;

    if ( m_authenticator.is_valid(req_ptr) ) {
        auto raw_method = FCGX_GetParam("REQUEST_METHOD", req_ptr->envp());

        if (!raw_method) {
            return m_handler_500;
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

            if (actual_method == HttpMethod::Not_a_method) return m_handler_405;

            HandlerMap_t &entry = it->second;

            auto h_it = entry.find(actual_method);
            if (h_it != entry.end()) {
                current_handler = h_it->second;
            } else {
                current_handler = m_handler_405;
            }
        } else {
            current_handler = m_handler_404;
        }
    } else {
        current_handler = m_handler_401;
    }

    return current_handler;
}

void DefaultDispatcher::add_endpoint(std::string uri, HttpMethod meth, std::shared_ptr<BasicHandler> handler) {
    if (!handler) throw std::invalid_argument("Endpoint pointer is null");

    add_end_slash(uri);

    auto endpoint_it = m_dispatch_matrix.find(uri);
    if (endpoint_it == m_dispatch_matrix.end()) {
        m_dispatch_matrix[uri] = HandlerMap_t { {meth, handler} };
    } else {
        HandlerMap_t &entry = endpoint_it->second;
        if ( entry.find(meth) != entry.end() ) {
            entry[meth] = handler;
        } else {
            throw std::invalid_argument("Endpoint with method already exist");
        }
    }
}