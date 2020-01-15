
#ifndef _HEADER_FILE_uri_dispatcher_20200103234120_
#define _HEADER_FILE_uri_dispatcher_20200103234120_

#include "fcgipp/http/method.hpp"
#include "fcgipp/http/default_handlers.hpp"
#include "basic_dispatcher.hpp"
#include "authenticator.hpp"
#include "fcgiapp.h"
#include "asio.hpp"
#include <unordered_map>
#include <memory>
#include <set>
#include <string>

namespace fcgipp {

    class DefaultDispatcher : public BasicDispatcher {
    public:
        DefaultDispatcher(BasicAuthenticator &auth) : m_authenticator(auth) {}
        ~DefaultDispatcher() = default;

        std::shared_ptr<BasicHandler> dispatch(std::shared_ptr<FcgiReqRes> req_ptr) override;

        void add_endpoint(std::string uri, HttpMethod, std::shared_ptr<BasicHandler>);

        void add_get(std::string uri, std::shared_ptr<BasicHandler> req) {
            add_endpoint(uri, HttpMethod::Get, req);
        }

        void add_post(std::string uri, std::shared_ptr<BasicHandler> req) {
            add_endpoint(uri, HttpMethod::Post, req);
        }

        void add_options(std::string uri, std::shared_ptr<BasicHandler> req) {
            add_endpoint(uri, HttpMethod::Options, req);
        }

        void add_patch(std::string uri, std::shared_ptr<BasicHandler> req) {
            add_endpoint(uri, HttpMethod::Patch, req);
        }

        void add_put(std::string uri, std::shared_ptr<BasicHandler> req) {
            add_endpoint(uri, HttpMethod::Put, req);
        }

        void add_delete(std::string uri, std::shared_ptr<BasicHandler> req) {
            add_endpoint(uri, HttpMethod::Delete, req);
        }

    private:

        void add_end_slash(std::string &uri) {
            if ( uri.size() == 0 ) return;
            if ( uri.at(uri.size() - 1) != '/' ) {
                uri = uri + "/";
            }
        }

        using HandlerMap_t = std::unordered_map<HttpMethod, std::shared_ptr<BasicHandler>>;
        std::unordered_map<std::string, HandlerMap_t> m_dispatch_matrix;

        std::shared_ptr<BasicHandler> m_handler_500 = std::make_shared<DefaultHttpInternalServerErrorHandler>();
        std::shared_ptr<BasicHandler> m_handler_404 = std::make_shared<DefaultHttpNotFoundHandler>();
        std::shared_ptr<BasicHandler> m_handler_401 = std::make_shared<DefaultHttpUnauthorizedHandler>();
        std::shared_ptr<BasicHandler> m_handler_405 = std::make_shared<DefaultHttpMethodNotAllowedHandler>();

        BasicAuthenticator &m_authenticator;
    };
};

#endif