

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

    template<typename Authenticator_t>
    Authenticator_t make_authenticator() {
        return Authenticator_t();
    }
};


#endif