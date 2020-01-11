
#ifndef _HEADER_FILE_handlers_20200103235030_
#define _HEADER_FILE_handlers_20200103235030_

#include "basic_handler.hpp"
#include "fcgiapp.h"

namespace fcgipp {
    class DefaultNotFoundHandler : public BasicHandler {
    public:
        DefaultNotFoundHandler() : BasicHandler(HttpMethod::ANY_METHOD) {}

        void handle(std::shared_ptr<fcgipp::FcgiReqRes> req) {
            FCGX_PutS(
                    "HTTP/1.1 404 Not Found\r\n"
                    "Status: 404 Not Found\r\n"
                    "Content-type: text/html\r\n"
                    "\r\n"
                    "<html>"
                        "<head><title>404 Not Found</title><meta charset=\"UTF-8\"></head>"
                        "<body><h1>404 Not found</h1></body>"
                    "</html>\r\n",
                    req->out_raw()
            );
        }
    };

    class DefaultUnauthorizedHandler : public BasicHandler {
    public:
        DefaultUnauthorizedHandler() : BasicHandler(HttpMethod::ANY_METHOD) {}

        void handle(std::shared_ptr<fcgipp::FcgiReqRes> req) {
            FCGX_PutS(
                    "HTTP/1.1 401 Unauthorized\r\n"
                    "Status: 401 Unauthorized\r\n"
                    "Content-type: text/html\r\n"
                    "\r\n"
                    "<html>"
                        "<head><title>401 Unauthorized</title><meta charset=\"UTF-8\"></head>"
                        "<body><h1>401 Unauthorized</h1></body>"
                    "</html>\r\n",
                    req->out_raw()
            );
        }
    };
};

#endif