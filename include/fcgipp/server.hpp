
#ifndef _HEADER_FILE_application_server_20200103163952_
#define _HEADER_FILE_application_server_20200103163952_

#include "req_res.hpp"
#include "fcgiapp.h"
#include "dispatcher.hpp"

namespace fcgipp {

    template<typename Async_t, typename Dispatcher_t>
    class FcgiApplicationServer {
        Async_t &m_async_scheduler;
        Dispatcher_t m_dispatcher;

    public:
        FcgiApplicationServer(Async_t &async_scheduler)
            : m_async_scheduler(async_scheduler)
            , m_dispatcher(Dispatcher_t{async_scheduler}) {
            FCGX_Init();
        }

        void operator() () {
            auto request = FcgiReqRes::create();

            if ( request->accept() ) {
                m_dispatcher.dispatch(request);

                m_async_scheduler.post(*this);
            }
        }

        Dispatcher_t &get_dispatcher() {
            return m_dispatcher;
        }
    };

    template<typename Async_t, typename Dispatcher_t = UriDispatcher<Async_t>>
    FcgiApplicationServer<Async_t, Dispatcher_t> make_server(Async_t &async) {
        return FcgiApplicationServer<Async_t, Dispatcher_t>(async);
    }
};

#endif