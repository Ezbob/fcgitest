

#include "fcgipp/default_authenticator.hpp"

using namespace fcgipp;

bool DefaultAuthenticator::is_valid(std::shared_ptr<BasicServerRequestResponse> const &) {
    return true;
}
