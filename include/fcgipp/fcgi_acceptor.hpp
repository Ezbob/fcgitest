
#ifndef _HEADER_FILE_application_server_20200103163952_
#define _HEADER_FILE_application_server_20200103163952_

#include <memory>
#include "fcgi_server_request_response.hpp"
#include "default_dispatcher.hpp"
#include "fcgiapp.h"

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
                auto request = std::make_shared<FcgiServerRequestResponse>();

                if ( request->accept() ) {
                    m_dispatcher.dispatch(request);
                }
            }
        }

    };
};

#endif