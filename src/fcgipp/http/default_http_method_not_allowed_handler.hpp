
#pragma once

#include <memory>
#include "fcgipp/basic_server_request_response.hpp"
#include "fcgipp/basic_handler.hpp"

namespace fcgipp {

    struct DefaultHttpMethodNotAllowedHandler : public BasicHandler {
        void handle(std::shared_ptr<fcgipp::BasicServerRequestResponse> req) {
            req->answerWith(
                    "HTTP/1.1 405 Method Not Allowed\r\n"
                    "Status: 405 Method Not Allowed\r\n"
                    "Content-type: text/html\r\n"
                    "\r\n"
                    "<html>"
                        "<head><title>405 Method Not Allowed</title><meta charset=\"UTF-8\"></head>"
                        "<body><h1>405 Method Not Allowed</h1></body>"
                    "</html>\r\n"
            );
        }
    };

}; // namespace fcgipp
