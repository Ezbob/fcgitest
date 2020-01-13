
#ifndef _HEADER_FILE_uri_dispatcher_20200103234120_
#define _HEADER_FILE_uri_dispatcher_20200103234120_

#include "basic_handler.hpp"
#include "fcgipp/http/method.hpp"
#include "fcgipp/http/default_handlers.hpp"
#include "authenticator.hpp"
#include "fcgiapp.h"
#include <unordered_map>
#include <memory>
#include <set>
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
                    HttpMethod actual_method = string_to_httpmethod(raw_method);

                    if (actual_method == HttpMethod::Not_a_method) return;

                    Entry_t const& entry = it->second;
                    std::set<HttpMethod> const& method_set = entry.first;

                    auto meth_it = method_set.find(actual_method);
                    if (meth_it != method_set.end()) {
                        current_handler = entry.second;
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

        void add_endpoint(std::string uri, HttpMethod meth, std::shared_ptr<BasicHandler> req) {
            add_end_slash(uri);
            auto endpoint_it = m_dispatch_matrix.find(uri);
            if (endpoint_it == m_dispatch_matrix.end()) {
                std::set<HttpMethod> m = {meth};
                Entry_t new_entry = { m, req };
                m_dispatch_matrix[uri] = new_entry;
            } else {
                auto &method_set = endpoint_it->second.first;

                auto method_it = method_set.find(meth);
                if ( method_it == method_set.end() ) {
                    method_set.emplace(meth);
                } else {
                    throw std::invalid_argument("Endpoint with method already exist");
                }
            }
        }

        void add_get(std::string uri, std::shared_ptr<BasicHandler> req) {
            add_endpoint(uri, HttpMethod::Get, req);
        }

        void add_post(std::string uri, std::shared_ptr<BasicHandler> req) {
            add_endpoint(uri, HttpMethod::Post, req);
        }

    private:

        void add_end_slash(std::string &uri) {
            if ( uri.size() == 0 ) return;
            if ( uri.at(uri.size() - 1) != '/' ) {
                uri = uri + "/";
            }
        }

        using Entry_t = std::pair<std::set<HttpMethod>, std::shared_ptr<BasicHandler>>;
        std::unordered_map<std::string, Entry_t> m_dispatch_matrix;

        std::shared_ptr<BasicHandler> m_handler_404 = std::shared_ptr<DefaultHttpNotFoundHandler>();
        std::shared_ptr<BasicHandler> m_handler_401 = std::shared_ptr<DefaultHttpUnauthorizedHandler>();

        Async_t &m_async_scheduler;
        BasicAuthenticator &m_authenticator;
    };
};

#endif