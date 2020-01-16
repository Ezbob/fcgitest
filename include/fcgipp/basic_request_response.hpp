
#ifndef _HEADER_FILE_basic_request_response_20200115205041_
#define _HEADER_FILE_basic_request_response_20200115205041_

#include <vector>
#include "basic_response.hpp"

namespace fcgipp {

    struct BasicRequestResponse {
        virtual ~BasicRequestResponse() = default;
        virtual bool accept() = 0;
        virtual bool is_accepted() = 0;
        virtual int answerWith(BasicResponse &) = 0;
        virtual int answerWith(std::string const&) = 0;
        virtual char const *get_parameter(std::string const &) const = 0;
        virtual const std::vector<const char *> get_parameters() const = 0;
    };

};

#endif
