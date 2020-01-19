
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
        std::unique_ptr<BasicAuthenticator> m_authenticator;
        std::unique_ptr<BasicMultiplexer> m_multiplexer;
        std::unique_ptr<BasicDispatcher> m_dispatcher;
        std::unique_ptr<BasicAcceptor> m_acceptor;

    public:
        FcgiApplication(
            std::unique_ptr<BasicMultiplexer> multi,
            std::unique_ptr<BasicAuthenticator> auth,
            std::unique_ptr<BasicDispatcher> dispatch,
            std::unique_ptr<BasicAcceptor> acceptor
        ) 
        : m_authenticator(std::move(auth))
        , m_multiplexer(std::move(multi))
        , m_dispatcher(std::move(dispatch))
        , m_acceptor(std::move(acceptor))
        {}

        FcgiApplication(
            std::unique_ptr<BasicMultiplexer> multi,
            std::unique_ptr<BasicAuthenticator> auth = std::unique_ptr<BasicAuthenticator>(new DefaultAuthenticator())
        ) 
        : m_authenticator(std::move(auth))
        , m_multiplexer(std::move(multi))
        , m_dispatcher(new DefaultDispatcher(*auth, *multi))
        , m_acceptor(new FcgiAcceptor(*m_dispatcher))
        {}

        FcgiApplication(
            BasicMultiplexer *multi,
            BasicAuthenticator *auth = (new DefaultAuthenticator())
        ) 
        : m_authenticator(std::move(auth))
        , m_multiplexer(multi)
        , m_dispatcher(new DefaultDispatcher(*auth, *multi))
        , m_acceptor(new FcgiAcceptor(*m_dispatcher))
        {}

        BasicDispatcher &get_dispatcher() {
            return *m_dispatcher;
        }

        BasicAcceptor &get_acceptor() {
            return *m_acceptor;
        }

        void start_accepting() {
            std::thread accepting_thread([this] {
                m_acceptor->start_accepting();
            });

            accepting_thread.detach();
        }
    };
};
