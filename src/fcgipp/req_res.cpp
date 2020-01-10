#include "fcgipp/req_res.hpp"

using namespace fcgipp;

std::shared_ptr<FcgiReqRes> FcgiReqRes::create() {
    return std::make_shared<FcgiReqRes>();
}
