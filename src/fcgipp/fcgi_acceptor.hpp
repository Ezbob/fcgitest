
#pragma once

#include <memory>
#include <thread>
#include "fcgipp/basic_acceptor.hpp"
#include "fcgipp/basic_multiplexer.hpp"
#include "fcgipp/fcgi_server_request_response.hpp"
#include "fcgipp/default_dispatcher.hpp"
#include "fcgiapp.h"

namespace fcgipp {

    class FcgiAcceptor : public BasicAcceptor {
        BasicDispatcher &m_dispatcher;
        BasicMultiplexer &m_scheduler;

        std::unique_ptr<std::thread> m_accept_thread;

        void start_accepting();
    public:
        FcgiAcceptor(BasicDispatcher &dispatch, BasicMultiplexer &multi);

        void start() override;
    };
};
