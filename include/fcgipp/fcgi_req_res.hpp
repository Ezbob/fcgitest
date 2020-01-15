
#ifndef _HEADER_FILE_fcgipp_20200102160204_
#define _HEADER_FILE_fcgipp_20200102160204_

#include <sstream>
#include <iostream>
#include <memory>
#include <cstring>
#include "basic_response.hpp"
#include "fcgiapp.h"

namespace fcgipp {
    /**
     * C++11 shared_ptr wrapper for FCGI requests.
     *
     * For convenience sake, a static factory `create` method is included
     * to create new FcgiReqRes instances that are wrapped in a
     * shared_ptr. A FcgiReqRes instance can also create more shared_ptr to itself by the `get` method.
     *
     * When the reference count reaches zero, the request is finished if it has been accepted.
     */
    class FcgiReqRes : std::enable_shared_from_this<FcgiReqRes> {
        FCGX_Request m_request;
        bool m_is_accepted;

    public:
        FcgiReqRes() : m_is_accepted(false) {
            FCGX_InitRequest(&m_request, 0, 0);
        }

        ~FcgiReqRes() {
            if (m_is_accepted) {
                FCGX_Finish_r(&m_request);
            }
        }

        static std::shared_ptr<FcgiReqRes> create();

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

        char **envp() {
            return m_request.envp;
        }

        int answerWith(BasicResponse &res) {
            auto r = res.render();
            return FCGX_PutStr(r.c_str(), r.size(), m_request.out);
        }

        int answerWith(std::string &res) {
            return FCGX_PutStr(res.c_str(), res.size(), m_request.out);
        }

        int answerWith(const char *res) {
            return FCGX_PutStr(res, std::strlen(res), m_request.out);
        }

        int log(std::string &res) {
            return FCGX_PutStr(res.c_str(), res.size(), m_request.err);
        }

        std::shared_ptr<FcgiReqRes> get() {
            return shared_from_this();
        }
    };
};

#endif
