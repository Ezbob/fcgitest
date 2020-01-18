
#pragma once

#include <sstream>
#include <iostream>
#include <memory>
#include <cstring>
#include <vector>
#include "basic_response.hpp"
#include "basic_server_request_response.hpp"
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
    class FcgiServerRequestResponse : public BasicServerRequestResponse, 
                                public std::enable_shared_from_this<FcgiServerRequestResponse> {
        FCGX_Request m_request;
        bool m_is_accepted;

    public:
        FcgiServerRequestResponse();

        ~FcgiServerRequestResponse();

        bool accept();

        bool is_accepted();

        int answerWith(BasicResponse &res);

        int answerWith(std::string const &res);

        int log(std::string &res);

        char const *get_parameter(std::string const &name) const override;

        const std::vector<const char *> get_parameters() const override;

        std::shared_ptr<FcgiServerRequestResponse> get();
    };

};
