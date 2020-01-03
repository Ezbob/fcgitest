
#ifndef _HEADER_FILE_uri_dispatcher_20200103234120_
#define _HEADER_FILE_uri_dispatcher_20200103234120_

#include "asio.hpp"
#include "basic_handler.hpp"
#include "handlers.hpp"
#include "fcgiapp.h"
#include <map>
#include <memory>
#include <string>

class UriDispatcher {

public:
    UriDispatcher(asio::io_context &io, BasicHandler::Ptr_Type not_found_handler)
        : m_io_ctx(io), m_not_found_handler_ptr(not_found_handler) {}

    UriDispatcher(asio::io_context &io)
        : m_io_ctx(io), m_not_found_handler_ptr(make_handler<DefaultNotFoundHandler>()) {}

    void dispatch(BasicHandler::Request_Ptr_Type req_ptr);

    void add_handler(std::string uri, BasicHandler::Ptr_Type req);

private:
    std::map<std::string, BasicHandler::Ptr_Type> m_dispatch_matrix;
    asio::io_context &m_io_ctx;
    BasicHandler::Ptr_Type m_not_found_handler_ptr;
};

#endif