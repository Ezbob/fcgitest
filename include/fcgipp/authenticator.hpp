

#ifndef _HEADER_FILE_authenticator_20200103164302_
#define _HEADER_FILE_authenticator_20200103164302_

#include "req_res.hpp"
#include <memory>

namespace fcgipp {

    struct BasicAuthenticator {
        virtual ~BasicAuthenticator() = default;
        virtual bool is_valid(std::shared_ptr<FcgiReqRes> const&) = 0;
    };

    class DefaultAuthenticator : public BasicAuthenticator {
    public:
        ~DefaultAuthenticator() = default;

        bool is_valid(std::shared_ptr<FcgiReqRes> const &) {
            return true;
        }
    };
};

#endif