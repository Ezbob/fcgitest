
#pragma once

#include <memory>
#include "fcgipp/basic_acceptor.hpp"
#include "fcgipp/fcgi_server_request_response.hpp"
#include "fcgipp/default_dispatcher.hpp"
#include "fcgiapp.h"

namespace fcgipp {

    class FcgiAcceptor : public BasicAcceptor {
        BasicDispatcher &m_dispatcher;

    public:
        FcgiAcceptor(BasicDispatcher &dispatch);

        void start_accepting() override;
    };
};
