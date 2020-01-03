#include "fcgipp/request.hpp"

std::shared_ptr<FcgiRequest> FcgiRequest::create() {
    return std::make_shared<FcgiRequest>();
}
