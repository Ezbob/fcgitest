
#ifndef _HEADER_FILE_basic_request_response_20200115205041_
#define _HEADER_FILE_basic_request_response_20200115205041_

#include "basic_response.hpp"

namespace fcgipp {

    struct BasicRequestResponse {
        virtual ~BasicRequestResponse() = default;
        virtual bool accept() = 0;
        virtual bool is_accepted() = 0;
        virtual int answerWith(BasicResponse &) = 0;
        virtual int answerWith(std::string const&) = 0;
    };

};

#endif
