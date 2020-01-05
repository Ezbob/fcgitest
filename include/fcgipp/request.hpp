
#ifndef _HEADER_FILE_fcgipp_20200102160204_
#define _HEADER_FILE_fcgipp_20200102160204_

#include <sstream>
#include <iostream>
#include <memory>
#include "fcgiapp.h"
#include "fcgio.h"

namespace fcgipp {
    /**
     * C++11 shared_ptr wrapper for FCGI requests.
     *
     * For convenience sake, a static factory `create` method is included
     * to create new FcgiRequest instances that are wrapped in a
     * shared_ptr. A FcgiRequest instance can also create more shared_ptr to itself by the `get` method.
     *
     * When the reference count reaches zero, the request is finished if it has been accepted.
     */
    class FcgiRequest : std::enable_shared_from_this<FcgiRequest> {
        FCGX_Request m_request;
        bool m_is_accepted;

        fcgi_streambuf m_in_buf;
        fcgi_streambuf m_out_buf;
        fcgi_streambuf m_err_buf;

        std::istream m_in_stream;
        std::ostream m_out_stream;
        std::ostream m_err_stream;
    public:
        FcgiRequest()
            : m_is_accepted(false) 
            , m_in_stream(&m_in_buf)
            , m_out_stream(&m_out_buf)
            , m_err_stream(&m_err_buf)
        {
            FCGX_InitRequest(&m_request, 0, 0);

            m_in_buf.attach(m_request.in);
            m_out_buf.attach(m_request.out);
            m_err_buf.attach(m_request.err);
        }

        ~FcgiRequest() {
            if (m_is_accepted) {
                m_out_stream.flush();
                m_err_stream.flush();
                FCGX_Finish_r(&m_request);
            }
        }

        static std::shared_ptr<FcgiRequest> create();

        bool accept() {
            m_is_accepted = (FCGX_Accept_r(&m_request) == 0);
            return m_is_accepted;
        }

        bool is_accepted() {
            return m_is_accepted;
        }

        FCGX_Stream *out_raw() {
            return m_request.out;
        }

        FCGX_Stream *err_raw() {
            return m_request.err;
        }

        FCGX_Stream *in_raw() {
            return m_request.in;
        }

        std::ostream &out() {
            return m_out_stream;
        }

        std::ostream &err() {
            return m_err_stream;
        }

        std::istream &in() {
            return m_in_stream;
        }

        char **envp() {
            return m_request.envp;
        }

        std::shared_ptr<FcgiRequest> get() {
            return shared_from_this();
        }
    };
};

#endif
