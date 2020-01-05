
#ifndef _HEADER_FILE_handlers_20200103235030_
#define _HEADER_FILE_handlers_20200103235030_

#include "basic_handler.hpp"
#include "fcgiapp.h"

namespace fcgipp {
    class DefaultNotFoundHandler : public BasicHandler {
    public:
        void handle(Request_Ptr_Type req) {
            FCGX_PutS("HTTP/1.1 404 Not Found\r\nStatus: 404 Not Found\r\nContent-type: text/html\r\n\r\n"
                    "<html><body><H1>404 Not found</H1></body></html>\n", req->out_raw());
        }
    };

    class DefaultUnauthorizedHandler : public BasicHandler {
    public:
        void handle(Request_Ptr_Type req) {
            FCGX_PutS("HTTP/1.1 401 Unauthorized\r\nStatus: 401 Unauthorized\r\nContent-type: text/html\r\n\r\n"
                    "<html><body><H1>401 Unauthorized</H1></body></html>\n", req->out_raw());
        }
    };
};

#endif