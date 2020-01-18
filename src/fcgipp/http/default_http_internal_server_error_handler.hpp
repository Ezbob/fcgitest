
#pragma once

#include "fcgipp/basic_handler.hpp"
#include "fcgipp/basic_server_request_response.hpp"
#include <memory>

namespace fcgipp {

    struct DefaultHttpInternalServerErrorHandler : public BasicHandler {
        void handle(std::shared_ptr<fcgipp::BasicServerRequestResponse> req) {
            req->answerWith(
                    "HTTP/1.1 500 Internal Server Error\r\n"
                    "Status: 500 Internal Server Error\r\n"
                    "Content-type: text/html\r\n"
                    "\r\n"
                    "<html>"
                        "<head><title>500 Internal Server Error</title><meta charset=\"UTF-8\"></head>"
                        "<body><h1>500 Internal Server Error</h1></body>"
                    "</html>\r\n"
            );
        }
    };

}; // namespace fcgipp