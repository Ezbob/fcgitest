
#ifndef _HEADER_FILE_application_server_20200103163952_
#define _HEADER_FILE_application_server_20200103163952_

#include "request.hpp"
#include "asio.hpp"

template<typename Dispatcher_t, typename Authenticator_t>
class FcgiApplicationServer {

    asio::io_context &m_io;
    Dispatcher_t &m_dispatcher;
    Authenticator_t &m_authenticator;

public:
    FcgiApplicationServer(asio::io_context &io, Authenticator_t &a, Dispatcher_t &d)
        : m_io(io), m_dispatcher(d), m_authenticator(a) {}

    void operator() () {
        auto request = FcgiRequest::create();

        if ( request->accept() ) {
            if ( m_authenticator.is_authentic(request) ) {
                m_dispatcher.dispatch(request);
            }
            m_io.post(*this);
        }
    }
};

#endif