

#ifndef _HEADER_FILE_authenticator_20200103164302_
#define _HEADER_FILE_authenticator_20200103164302_

#include "request.hpp"
#include <memory>

namespace fcgipp {
    struct DefaultAuthenticator {
        bool is_valid(std::shared_ptr<FcgiRequest> const &) {
            return true;
        }
    };
};


#endif