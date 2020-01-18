
#pragma once

#include "fcgipp/basic_handler.hpp"
#include "fcgipp/basic_server_request_response.hpp"
#include <memory>

namespace fcgipp {

    struct DefaultHttpUnauthorizedHandler : public BasicHandler {
        void handle(std::shared_ptr<fcgipp::BasicServerRequestResponse> req) override;
    };

}; // namespace fcgipp
