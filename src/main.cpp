#include <stdlib.h>
#ifdef _WIN32
#include <process.h>
#else
#include <unistd.h>
extern char ** environ;
#endif

#include "fcgiapp.h"
#include <sstream>
#include "fcgipp/fcgi.hpp"
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
    fcgipp::HttpResponse resp;
    std::ostream &out = resp.body();
public:
    void handle(std::shared_ptr<fcgipp::FcgiRequest> req) {

        resp.put_header("Content-type", "text/html");

        out << "<TITLE>echo-cpp</TITLE>"
                "<H1>echo-cpp</H1>"
                "<H4>PID: " << g_pid << "</H4>"
                "<H4>Request Number: " << ++g_count << "</H4>";

        out << "<H4>Process/Initial Environment</H4>\n";
        penv(environ, out);

        out << "<H4>Fcgi Environment</H4>\n";
        penv(req->envp(), out);

        req->answerWith(resp);
    }
};

int main(void) {

    if ( FCGX_Init() != 0 ) return 1;

    asio::io_context io;

    auto authenticator = fcgipp::DefaultAuthenticator();

    auto dispatcher = fcgipp::UriDispatcher<asio::io_context>(io, authenticator);

    auto root_handler = fcgipp::make_handler<MyHandler>();

    dispatcher.add_endpoint("/", root_handler);

    auto server = fcgipp::make_server(io, dispatcher);

    io.post(server);

    io.run();

    return 0;
}
