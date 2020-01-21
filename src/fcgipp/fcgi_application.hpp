
#pragma once

#include <memory>
#include <thread>
#include "fcgipp/basic_acceptor.hpp"
#include "fcgipp/basic_scheduler.hpp"
#include "fcgipp/basic_authenticator.hpp"
#include "fcgipp/basic_dispatcher.hpp"
#include "fcgipp/default_authenticator.hpp"
#include "fcgipp/default_dispatcher.hpp"
#include "fcgipp/fcgi_acceptor.hpp"

namespace fcgipp {
    class FcgiApplication {
    private:
        template<typename T>
        using Ptr = std::unique_ptr<T>;

        Ptr<BasicScheduler> m_async_scheduler;
        Ptr<BasicAuthenticator> m_authenticator;
        Ptr<BasicDispatcher> m_dispatcher;
        Ptr<BasicAcceptor> m_acceptor;

    public:
        FcgiApplication(
            std::unique_ptr<BasicScheduler>,
            std::unique_ptr<BasicAuthenticator> = std::unique_ptr<BasicAuthenticator>(),
            std::unique_ptr<BasicDispatcher> = std::unique_ptr<BasicDispatcher>(),
            std::unique_ptr<BasicAcceptor> = std::unique_ptr<BasicAcceptor>()
        );

        void add_get(std::string, std::shared_ptr<BasicHandler>);
        void add_post(std::string, std::shared_ptr<BasicHandler>);
        void add_options(std::string, std::shared_ptr<BasicHandler>);
        void add_patch(std::string, std::shared_ptr<BasicHandler>);
        void add_put(std::string, std::shared_ptr<BasicHandler>);
        void add_delete(std::string, std::shared_ptr<BasicHandler>);

        void start_nonblock();
    };
};
