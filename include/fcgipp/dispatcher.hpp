
#ifndef _HEADER_FILE_uri_dispatcher_20200103234120_
#define _HEADER_FILE_uri_dispatcher_20200103234120_

#include "basic_handler.hpp"
#include "http_handlers.hpp"
#include "http_method.hpp"
#include "authenticator.hpp"
#include "fcgiapp.h"
#include <unordered_map>
#include <memory>
#include <string>

namespace fcgipp {

    struct BasicDispatcher {
        virtual ~BasicDispatcher() = default;
        virtual void dispatch(std::shared_ptr<FcgiReqRes> req_ptr) = 0;
    };

    template<typename Async_t>
    class DefaultDispatcher : public BasicDispatcher {
    public:
        DefaultDispatcher(Async_t &aio, BasicAuthenticator &auth)
            : m_async_scheduler(aio)
            , m_authenticator(auth) {}

        ~DefaultDispatcher() = default;

        void dispatch(std::shared_ptr<FcgiReqRes> req_ptr) {
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
                    auto expected_method = it->second->get_method();
                    auto actual_method = string_to_httpmethod(raw_method);

                    if (expected_method == HttpMethod::Not_a_method) return;

                    if (expected_method == actual_method || expected_method == HttpMethod::Any_method) {
                        current_handler = it->second;
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

        void add_endpoint(std::string uri, std::shared_ptr<BasicHandler> req) {
            add_end_slash(uri);
            m_dispatch_matrix[uri] = req;
        }

    private:

        void add_end_slash(std::string &uri) {
            if ( uri.size() == 0 ) return;
            if ( uri.at(uri.size() - 1) != '/' ) {
                uri = uri + "/";
            }
        }

        std::unordered_map<std::string, std::shared_ptr<BasicHandler>> m_dispatch_matrix;

        std::shared_ptr<BasicHandler> m_handler_404 = std::shared_ptr<DefaultNotFoundHandler>();
        std::shared_ptr<BasicHandler> m_handler_401 = std::shared_ptr<DefaultUnauthorizedHandler>();

        Async_t &m_async_scheduler;
        BasicAuthenticator &m_authenticator;
    };
};

#endif