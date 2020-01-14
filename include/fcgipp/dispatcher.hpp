
#ifndef _HEADER_FILE_uri_dispatcher_20200103234120_
#define _HEADER_FILE_uri_dispatcher_20200103234120_

#include "basic_handler.hpp"
#include "fcgipp/http/method.hpp"
#include "fcgipp/http/default_handlers.hpp"
#include "authenticator.hpp"
#include "fcgiapp.h"
#include "asio.hpp"
#include <unordered_map>
#include <memory>
#include <set>
#include <string>

namespace fcgipp {
    struct BasicDispatcher {
        virtual ~BasicDispatcher() = default;
        virtual void dispatch(std::shared_ptr<FcgiReqRes> req_ptr) = 0;
    };

    class DefaultDispatcher : public BasicDispatcher {
    public:
        DefaultDispatcher(asio::io_context &aio, BasicAuthenticator &auth)
            : m_async_scheduler(aio)
            , m_authenticator(auth) {}

        ~DefaultDispatcher() = default;

        void dispatch(std::shared_ptr<FcgiReqRes> req_ptr) override;

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

        class HandlerEntry {
            std::set<HttpMethod> accepted_methods;
            std::shared_ptr<BasicHandler> handler;
        public:
            HandlerEntry(HttpMethod meth, std::shared_ptr<BasicHandler> h)
                : accepted_methods{meth}
                , handler(h) {}

            bool is_accepted_method(HttpMethod const& meth) {
                return accepted_methods.find(meth) != accepted_methods.end();
            }

            void add_accepted_method(HttpMethod meth) {
                accepted_methods.emplace(meth);
            }

            std::shared_ptr<BasicHandler> get_handler() {
                return handler;
            }
        };

        std::unordered_map<std::string, std::shared_ptr<HandlerEntry>> m_dispatch_matrix;

        std::shared_ptr<BasicHandler> m_handler_404 = std::shared_ptr<DefaultHttpNotFoundHandler>();
        std::shared_ptr<BasicHandler> m_handler_401 = std::shared_ptr<DefaultHttpUnauthorizedHandler>();

        asio::io_context &m_async_scheduler;
        BasicAuthenticator &m_authenticator;
    };
};

#endif