
#pragma once

#include <memory>
#include "fcgipp/basic_handler.hpp"
#include "fcgipp/basic_server_request_response.hpp"

namespace fcgipp {

    struct DefaultHttpNotFoundHandler : public BasicHandler {
        void handle(std::shared_ptr<fcgipp::BasicServerRequestResponse> req) override {
            req->answerWith(
                    "HTTP/1.1 404 Not Found\r\n"
                    "Status: 404 Not Found\r\n"
                    "Content-type: text/html\r\n"
                    "\r\n"
                    "<html>"
                        "<head><title>404 Not Found</title><meta charset=\"UTF-8\"></head>"
                        "<body><h1>404 Not found</h1></body>"
                    "</html>\r\n"
            );
        }
    };

}; // namespace fcgipp
