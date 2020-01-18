
#include "fcgipp/http/default_http_unauthorized_handler.hpp"

using namespace fcgipp;

void DefaultHttpUnauthorizedHandler::handle(std::shared_ptr<fcgipp::BasicServerRequestResponse> req) {
    req->respond_with(
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
