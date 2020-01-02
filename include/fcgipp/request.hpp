
#ifndef _HEADER_FILE_fcgipp_20200102160204_
#define _HEADER_FILE_fcgipp_20200102160204_

#include <memory>
#include "fcgiapp.h"

class FcgiRequest : std::enable_shared_from_this<FcgiRequest> {
    FCGX_Request m_request;
    bool m_is_accepted;

public:
    FcgiRequest() : m_is_accepted(false) {
        FCGX_InitRequest(&m_request, 0, 0);
    }

    ~FcgiRequest() {
        if (m_is_accepted) FCGX_Finish_r(&m_request);
    }

    operator FCGX_Request &() {
        return m_request;
    }

    static std::shared_ptr<FcgiRequest> create();

    bool accept() {
        m_is_accepted = (FCGX_Accept_r(&m_request) == 0);
        return m_is_accepted;
    }

    bool is_accepted() {
        return m_is_accepted;
    }

    FCGX_Stream *out() {
        return m_request.out;
    }

    FCGX_Stream *err() {
        return m_request.err;
    }

    FCGX_Stream *in() {
        return m_request.in;
    }

    char **envp() {
        return m_request.envp;
    }

    std::shared_ptr<FcgiRequest> get() {
        return shared_from_this();
    }
};

std::shared_ptr<FcgiRequest> FcgiRequest::create() {
    return std::make_shared<FcgiRequest>();
}

#endif