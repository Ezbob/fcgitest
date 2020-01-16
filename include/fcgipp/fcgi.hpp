#include "default_authenticator.hpp"
#include "default_dispatcher.hpp"
#include "basic_handler.hpp"
#include "basic_response.hpp"
#include "basic_multiplexer.hpp"
#include "fcgipp/http/default_handlers.hpp"
#include "fcgipp/http/method.hpp"
#include "fcgipp/http/response.hpp"

#include "fcgi_server_request_response.hpp"
#include "fcgi_acceptor.hpp"