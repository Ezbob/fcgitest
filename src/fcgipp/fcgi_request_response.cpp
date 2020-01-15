#include "fcgipp/fcgi_request_response.hpp"

using namespace fcgipp;

std::shared_ptr<FcgiReqRes> FcgiReqRes::create() {
    return std::make_shared<FcgiReqRes>();
}
