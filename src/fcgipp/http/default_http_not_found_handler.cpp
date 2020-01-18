
#include "fcgipp/http/default_http_not_found_handler.hpp"

using namespace fcgipp;

void DefaultHttpNotFoundHandler::handle(std::shared_ptr<fcgipp::BasicServerRequestResponse> req) {
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
