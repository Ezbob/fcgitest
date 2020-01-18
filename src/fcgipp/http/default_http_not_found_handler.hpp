
#pragma once

#include <memory>
#include "fcgipp/basic_handler.hpp"
#include "fcgipp/basic_server_request_response.hpp"

namespace fcgipp {

    struct DefaultHttpNotFoundHandler : public BasicHandler {
        void handle(std::shared_ptr<fcgipp::BasicServerRequestResponse>) override;
    };

}; // namespace fcgipp
