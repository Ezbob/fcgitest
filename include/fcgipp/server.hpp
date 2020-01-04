
#ifndef _HEADER_FILE_application_server_20200103163952_
#define _HEADER_FILE_application_server_20200103163952_

#include "request.hpp"
#include "asio.hpp"

template<typename Dispatcher_t>
class FcgiApplicationServer {
    asio::io_context &m_io;
    Dispatcher_t &m_dispatcher;
public:
    FcgiApplicationServer(asio::io_context &io, Dispatcher_t &d)
        : m_io(io)
        , m_dispatcher(d)
    {}

    void operator() () {
        auto request = FcgiRequest::create();

        if ( request->accept() ) {
            m_dispatcher.dispatch(request);

            m_io.post(*this);
        }
    }
};

template<typename Async_t, typename Dispatcher_t>
FcgiApplicationServer<Dispatcher_t> make_server(Async_t &async, Dispatcher_t &d) {
    return FcgiApplicationServer<Dispatcher_t>(async, d);
}

#endif