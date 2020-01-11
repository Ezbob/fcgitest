
#include "fcgipp/handler_response.hpp"

using namespace fcgipp;

std::ostream &operator <<(std::ostream &os, BasicResponse &response) {
    os << response.render();
    return os;
}
