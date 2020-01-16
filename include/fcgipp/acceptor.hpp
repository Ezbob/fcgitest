
#ifndef _HEADER_FILE_application_server_20200103163952_
#define _HEADER_FILE_application_server_20200103163952_

#include <memory>
#include "fcgi_request_response.hpp"
#include "fcgiapp.h"
#include "asio.hpp"
#include "dispatcher.hpp"

namespace fcgipp {

    class FcgiAcceptor {
        BasicDispatcher &m_dispatcher;

    public:
        FcgiAcceptor(BasicDispatcher &dispatch)
            : m_dispatcher(dispatch) {
            FCGX_Init();
        }

        void start_accepting() {
            while (true) {
                auto request = std::make_shared<FcgiRequestResponse>();

                if ( request->accept() ) {
                    m_dispatcher.dispatch(request);
                }
            }
        }

    };
};

#endif