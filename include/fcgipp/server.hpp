
#ifndef _HEADER_FILE_application_server_20200103163952_
#define _HEADER_FILE_application_server_20200103163952_

#include "req_res.hpp"
#include "fcgiapp.h"

namespace fcgipp {

    template<typename Async_t, typename Dispatcher_t>
    class FcgiApplicationServer {
        Async_t &m_async_scheduler;
        Dispatcher_t &m_dispatcher;
    public:
        FcgiApplicationServer(Async_t &async_scheduler, Dispatcher_t &d)
            : m_async_scheduler(async_scheduler)
            , m_dispatcher(d) {
            FCGX_Init();
        }

        void operator() () {
            auto request = FcgiReqRes::create();

            if ( request->accept() ) {
                m_dispatcher.dispatch(request);

                m_async_scheduler.post(*this);
            }
        }
    };

    template<typename Async_t, typename Dispatcher_t>
    FcgiApplicationServer<Async_t, Dispatcher_t> make_server(Async_t &async, Dispatcher_t &d) {
        return FcgiApplicationServer<Async_t, Dispatcher_t>(async, d);
    }
};

#endif