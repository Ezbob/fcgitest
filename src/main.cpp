#include <stdlib.h>
#ifdef _WIN32
#include <process.h>
#else
#include <unistd.h>
extern char ** environ;
#endif

#include "fcgiapp.h"
#include <sstream>
#include <map>
#include "fcgipp/request.hpp"
#include "fcgipp/basic_handler.hpp"
#include "fcgipp/authenticator.hpp"
#include "fcgipp/server.hpp"
#include "asio.hpp"

static void penv(const char * const * envp, std::stringstream &stream)
{
    stream << "<PRE>\n";
    for ( ; *envp; ++envp)
    {
        stream << *envp << "\n";
    }
    stream << "</PRE>\n";
}

static long g_pid = getpid();
static int g_count = 0;

class MyHandler : public BasicHandler {

public:
    void handle(Request_Ptr_Type req) {
        std::stringstream out;
        out << "Content-type: text/html\r\n\r\n"
                "<TITLE>echo-cpp</TITLE>\n"
                "<H1>echo-cpp</H1>\n"
                "<H4>PID: " << g_pid << "</H4>\n"
                "<H4>Request Number: " << ++g_count << "</H4>\n";

        out << "<H4>Process/Initial Environment</H4>\n";
        penv(environ, out);

        out << "<H4>Fcgi Environment</H4>\n";
        penv(req->envp(), out);

        auto val = out.str();

        FCGX_PutStr(val.c_str(), val.size(), req->out());
    }
};

class UriDispatcher {

public:
    UriDispatcher(asio::io_context &io) : m_io_ctx(io) {}

    void dispatch(BasicHandler::Request_Ptr_Type req_ptr) {
        auto uri = FCGX_GetParam("REQUEST_URI", req_ptr->envp());

        if (uri) {
            auto key = std::string(uri);
            auto it = m_dispatch_matrix.find(key);
            if ( it != m_dispatch_matrix.end() ) {
                BasicHandler::Ptr_Type handler_ptr = it->second;

                m_io_ctx.post([handler_ptr, req_ptr]() {
                    handler_ptr->handle(req_ptr);
                });
            }
        }
    }

    void add_handler(std::string uri, BasicHandler::Ptr_Type req) {
        m_dispatch_matrix[uri] = req;
    }

private:
    std::map<std::string, BasicHandler::Ptr_Type> m_dispatch_matrix;
    asio::io_context &m_io_ctx;
};

int main(void) {

    if ( FCGX_Init() != 0 ) return 1;

    asio::io_context io;

    auto dispatcher = UriDispatcher(io);
    auto authenticator = DefaultAuthenticator();

    auto root_handler = make_handler<MyHandler>();

    dispatcher.add_handler("/echo/", root_handler);
    dispatcher.add_handler("/echo", root_handler);

    auto server = make_server(io, authenticator, dispatcher);

    io.post(server);

    io.run();

    return 0;
}
