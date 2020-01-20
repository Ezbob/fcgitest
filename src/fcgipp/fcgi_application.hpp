
#pragma once

#include <memory>
#include <thread>
#include "fcgipp/basic_acceptor.hpp"
#include "fcgipp/basic_multiplexer.hpp"
#include "fcgipp/basic_authenticator.hpp"
#include "fcgipp/basic_dispatcher.hpp"
#include "fcgipp/default_authenticator.hpp"
#include "fcgipp/default_dispatcher.hpp"
#include "fcgipp/fcgi_acceptor.hpp"

namespace fcgipp {
    class FcgiApplication {

        template<typename T>
        using Ptr = std::unique_ptr<T>;

        Ptr<BasicMultiplexer> m_async_scheduler;
        Ptr<BasicAuthenticator> m_authenticator;
        Ptr<BasicDispatcher> m_dispatcher;
        Ptr<BasicAcceptor> m_acceptor;

    public:
        FcgiApplication(
            std::unique_ptr<BasicMultiplexer> sch,
            std::unique_ptr<BasicAuthenticator> auth = std::unique_ptr<BasicAuthenticator>(),
            std::unique_ptr<BasicDispatcher> dispatch = std::unique_ptr<BasicDispatcher>(),
            std::unique_ptr<BasicAcceptor> acceptor = std::unique_ptr<BasicAcceptor>()
        )
        : m_async_scheduler(std::move(sch))
        , m_authenticator(auth == nullptr ? Ptr<BasicAuthenticator>(new DefaultAuthenticator()) : std::move(auth) )
        , m_dispatcher(dispatch == nullptr ? Ptr<DefaultDispatcher>(new DefaultDispatcher(*auth)) : std::move(dispatch))
        , m_acceptor(acceptor == nullptr ? Ptr<BasicAcceptor>(new FcgiAcceptor(*m_dispatcher, *sch)) : std::move(acceptor))
        {}

        void add_get(std::string uri, std::shared_ptr<BasicHandler> req) {
            m_dispatcher->add_endpoint(uri, HttpMethod::Get, req);
        }

        void add_post(std::string uri, std::shared_ptr<BasicHandler> req) {
            m_dispatcher->add_endpoint(uri, HttpMethod::Post, req);
        }

        void add_options(std::string uri, std::shared_ptr<BasicHandler> req) {
            m_dispatcher->add_endpoint(uri, HttpMethod::Options, req);
        }

        void add_patch(std::string uri, std::shared_ptr<BasicHandler> req) {
            m_dispatcher->add_endpoint(uri, HttpMethod::Patch, req);
        }

        void add_put(std::string uri, std::shared_ptr<BasicHandler> req) {
            m_dispatcher->add_endpoint(uri, HttpMethod::Put, req);
        }

        void add_delete(std::string uri, std::shared_ptr<BasicHandler> req) {
            m_dispatcher->add_endpoint(uri, HttpMethod::Delete, req);
        }

        BasicDispatcher &get_dispatcher() {
            return *m_dispatcher;
        }

        BasicAcceptor &get_acceptor() {
            return *m_acceptor;
        }

        void start() {
            m_acceptor->start();
        }
    };
};
