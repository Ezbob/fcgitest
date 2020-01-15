
#ifndef _HEADER_FILE_basic_authenticator_20200115204611_
#define _HEADER_FILE_basic_authenticator_20200115204611_

#include <memory>
#include "fcgi_request_response.hpp"

namespace fcgipp {

    struct BasicAuthenticator {
        virtual ~BasicAuthenticator() = default;
        virtual bool is_valid(std::shared_ptr<FcgiReqRes> const&) = 0;
    };

};

#endif