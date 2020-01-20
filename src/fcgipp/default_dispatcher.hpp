
#pragma once

#include "fcgipp/http/method.hpp"
#include "fcgipp/http/default_http_internal_server_error_handler.hpp"
#include "fcgipp/http/default_http_method_not_allowed_handler.hpp"
#include "fcgipp/http/default_http_not_found_handler.hpp"
#include "fcgipp/http/default_http_unauthorized_handler.hpp"

#include "fcgipp/basic_dispatcher.hpp"
#include "fcgipp/basic_server_request_response.hpp"
#include "fcgipp/basic_authenticator.hpp"

#include <unordered_map>
#include <memory>
#include <string>

namespace fcgipp {

    class DefaultDispatcher : public BasicDispatcher {
    public:
        DefaultDispatcher(BasicAuthenticator &auth) 
            : m_authenticator(auth)
            {}

        ~DefaultDispatcher() = default;

        void dispatch(std::shared_ptr<BasicServerRequestResponse>) override;

        void add_endpoint(std::string uri, HttpMethod, std::shared_ptr<BasicHandler>) override;

    private:
        std::shared_ptr<BasicHandler> select(std::shared_ptr<BasicServerRequestResponse> req_ptr) const;

        std::string build_uri(const char *raw) const;
        void add_end_slash(std::string &uri) const;

        using HandlerMap_t = std::unordered_map<HttpMethod, std::shared_ptr<BasicHandler>>;
        std::unordered_map<std::string, HandlerMap_t> m_dispatch_matrix;

        std::shared_ptr<BasicHandler> m_handler_500 = std::make_shared<DefaultHttpInternalServerErrorHandler>();
        std::shared_ptr<BasicHandler> m_handler_404 = std::make_shared<DefaultHttpNotFoundHandler>();
        std::shared_ptr<BasicHandler> m_handler_401 = std::make_shared<DefaultHttpUnauthorizedHandler>();
        std::shared_ptr<BasicHandler> m_handler_405 = std::make_shared<DefaultHttpMethodNotAllowedHandler>();

        BasicAuthenticator &m_authenticator;
    };
};
