
#pragma once

#include "fcgipp/basic_handler.hpp"
#include "fcgipp/basic_server_request_response.hpp"
#include <memory>

namespace fcgipp {

    struct DefaultHttpUnauthorizedHandler : public BasicHandler {
        void handle(std::shared_ptr<fcgipp::BasicServerRequestResponse> req) {
            req->answerWith(
                    "HTTP/1.1 401 Unauthorized\r\n"
                    "Status: 401 Unauthorized\r\n"
                    "Content-type: text/html\r\n"
                    "\r\n"
                    "<html>"
                        "<head><title>401 Unauthorized</title><meta charset=\"UTF-8\"></head>"
                        "<body><h1>401 Unauthorized</h1></body>"
                    "</html>\r\n"
            );
        }
    };

}; // namespace fcgipp
