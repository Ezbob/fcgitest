
#ifndef _HEADER_FILE_application_server_20200103163952_
#define _HEADER_FILE_application_server_20200103163952_

#include "req_res.hpp"
#include "fcgiapp.h"
#include "dispatcher.hpp"

namespace fcgipp {

    template<typename Async_t>
    class FcgiAcceptor {
        Async_t &m_async_scheduler;
        BasicDispatcher &m_dispatcher;

        void accept() {
            auto request = FcgiReqRes::create();

            if ( request->accept() ) {
                m_dispatcher.dispatch(request);

                schedule_accept();
            }
        }

    public:
        FcgiAcceptor(Async_t &async_scheduler, BasicDispatcher &dispatch)
            : m_async_scheduler(async_scheduler)
            , m_dispatcher(dispatch) {
            FCGX_Init();
        }

        void schedule_accept() {
            m_async_scheduler.post([this] {
                accept();
            });
        }

    };
};

#endif