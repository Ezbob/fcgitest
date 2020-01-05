#include <stdlib.h>
#ifdef _WIN32
#include <process.h>
#else
#include <unistd.h>
extern char ** environ;
#endif

#include "fcgiapp.h"
#include <sstream>
#include "fcgipp/request.hpp"
#include "fcgipp/basic_handler.hpp"
#include "fcgipp/authenticator.hpp"
#include "fcgipp/dispatcher.hpp"
#include "fcgipp/server.hpp"
#include "asio.hpp"

static void penv(const char * const * envp, std::ostream &stream)
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

class MyHandler : public fcgipp::BasicHandler {

public:
    void handle(Request_Ptr_Type req) {
        auto out = std::stringstream();
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

        FCGX_PutS(val.c_str(), req->out_raw());
    }
};

int main(void) {

    if ( FCGX_Init() != 0 ) return 1;

    asio::io_context io;

    auto authenticator = fcgipp::make_authenticator<fcgipp::DefaultAuthenticator>();

    auto dispatcher = fcgipp::make_dispatcher<fcgipp::UriDispatcher>(io, authenticator);

    auto root_handler = fcgipp::make_handler<MyHandler>();

    dispatcher.add_endpoint("/", root_handler);

    auto server = fcgipp::make_server(io, dispatcher);

    io.post(server);

    io.run();

    return 0;
}
