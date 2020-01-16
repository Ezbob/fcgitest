
#ifndef _HEADER_FILE_fcgipp_20200102160204_
#define _HEADER_FILE_fcgipp_20200102160204_

#include <sstream>
#include <iostream>
#include <memory>
#include <cstring>
#include <vector>
#include "basic_response.hpp"
#include "basic_request_response.hpp"
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
    class FcgiRequestResponse : public BasicRequestResponse, 
                                public std::enable_shared_from_this<FcgiRequestResponse> {
        FCGX_Request m_request;
        bool m_is_accepted;

    public:
        FcgiRequestResponse() : m_is_accepted(false) {
            FCGX_InitRequest(&m_request, 0, 0);
        }

        ~FcgiRequestResponse() {
            if (m_is_accepted) {
                FCGX_Finish_r(&m_request);
            }
        }

        bool accept() {
            m_is_accepted = (FCGX_Accept_r(&m_request) == 0);
            return m_is_accepted;
        }

        bool is_accepted() {
            return m_is_accepted;
        }

        int answerWith(BasicResponse &res) {
            auto r = res.render();
            return FCGX_PutStr(r.c_str(), r.size(), m_request.out);
        }

        int answerWith(std::string const &res) {
            return FCGX_PutStr(res.c_str(), res.size(), m_request.out);
        }

        int log(std::string &res) {
            return FCGX_PutStr(res.c_str(), res.size(), m_request.err);
        }

        char const *get_parameter(std::string const &name) const override {
            return FCGX_GetParam(name.c_str(), m_request.envp);
        }

        const std::vector<const char *> get_parameters() const override {
            std::vector<const char *> res;
            for (size_t i = 0; m_request.envp[i] != nullptr; ++i) 
                res.push_back(m_request.envp[i]);
            return res;
        }

        std::shared_ptr<FcgiRequestResponse> get() {
            return shared_from_this();
        }
    };

};

#endif
