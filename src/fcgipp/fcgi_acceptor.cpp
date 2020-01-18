
#include "fcgipp/fcgi_acceptor.hpp"

using namespace fcgipp;

FcgiAcceptor::FcgiAcceptor(BasicDispatcher &dispatch)
    : m_dispatcher(dispatch) {
    FCGX_Init();
}

void FcgiAcceptor::start_accepting() {
    while (true) {
        auto request = std::make_shared<FcgiServerRequestResponse>();

        if ( request->accept() ) {
            m_dispatcher.dispatch(request);
        }
    }
}
