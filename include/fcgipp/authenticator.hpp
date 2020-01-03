

#ifndef _HEADER_FILE_authenticator_20200103164302_
#define _HEADER_FILE_authenticator_20200103164302_

#include "request.hpp"
#include <memory>

struct DefaultAuthenticator {
    bool is_authentic(std::shared_ptr<FcgiRequest> const &) {
        return true;
    }
};

#endif