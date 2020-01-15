
#ifndef _HEADER_FILE_default_handlers_20200113205909_
#define _HEADER_FILE_default_handlers_20200113205909_

#include "fcgipp/basic_handler.hpp"
#include "fcgipp/fcgi_req_res.hpp"
#include "fcgiapp.h"
#include <memory>

namespace fcgipp {
    struct DefaultHttpNotFoundHandler : public BasicHandler {
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

    struct DefaultHttpMethodNotAllowedHandler : public BasicHandler {
        void handle(std::shared_ptr<fcgipp::FcgiReqRes> req) {
            FCGX_PutS(
                    "HTTP/1.1 405 Method Not Allowed\r\n"
                    "Status: 405 Method Not Allowed\r\n"
                    "Content-type: text/html\r\n"
                    "\r\n"
                    "<html>"
                        "<head><title>405 Method Not Allowed</title><meta charset=\"UTF-8\"></head>"
                        "<body><h1>405 Method Not Allowed</h1></body>"
                    "</html>\r\n",
                    req->out_raw()
            );
        }
    };

    struct DefaultHttpInternalServerErrorHandler : public BasicHandler {
        void handle(std::shared_ptr<fcgipp::FcgiReqRes> req) {
            FCGX_PutS(
                    "HTTP/1.1 500 Internal Server Error\r\n"
                    "Status: 500 Internal Server Error\r\n"
                    "Content-type: text/html\r\n"
                    "\r\n"
                    "<html>"
                        "<head><title>500 Internal Server Error</title><meta charset=\"UTF-8\"></head>"
                        "<body><h1>500 Internal Server Error</h1></body>"
                    "</html>\r\n",
                    req->out_raw()
            );
        }
    };

    struct DefaultHttpUnauthorizedHandler : public BasicHandler {
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