#include "fcgipp/request.hpp"

using namespace fcgipp;

std::shared_ptr<FcgiRequest> FcgiRequest::create() {
    return std::make_shared<FcgiRequest>();
}
