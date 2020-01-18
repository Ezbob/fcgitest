
#pragma once

#include <memory>
#include "fcgipp/basic_server_request_response.hpp"
#include "fcgipp/basic_handler.hpp"

namespace fcgipp {

    struct DefaultHttpMethodNotAllowedHandler : public BasicHandler {
        void handle(std::shared_ptr<fcgipp::BasicServerRequestResponse>) override;
    };

}; // namespace fcgipp
