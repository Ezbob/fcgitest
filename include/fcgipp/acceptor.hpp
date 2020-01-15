
#ifndef _HEADER_FILE_application_server_20200103163952_
#define _HEADER_FILE_application_server_20200103163952_

#include <memory>
#include "fcgi_request_response.hpp"
#include "fcgiapp.h"
#include "asio.hpp"
#include "dispatcher.hpp"

namespace fcgipp {

    class FcgiAcceptor {
        asio::io_context &m_async_scheduler;
        BasicDispatcher &m_dispatcher;

    public:
        FcgiAcceptor(asio::io_context &async_scheduler, BasicDispatcher &dispatch)
            : m_async_scheduler(async_scheduler)
            , m_dispatcher(dispatch) {
            FCGX_Init();
        }

        void start_accepting() {
            while (true) {
                auto request = std::make_shared<FcgiRequestResponse>();

                if ( request->accept() ) {
                    auto handler = m_dispatcher.dispatch(request);

                    m_async_scheduler.post([handler, request] {
                        handler->handle(request);
                    });
                }
            }
        }

    };
};

#endif