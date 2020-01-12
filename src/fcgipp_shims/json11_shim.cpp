
#include "fcgipp_shims/json11_shim.hpp"
#include <ostream>
#include "json11.hpp"

std::ostream &operator<<(std::ostream &os, json11::Json &j) {
    return (os << j.dump());
}
