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

class Handler {
    std::shared_ptr<FcgiRequest> m_request;
public:
    Handler(std::shared_ptr<FcgiRequest> r) : m_request(r) {}

    void operator() () {
        std::stringstream out;
        out << "Content-type: text/html\r\n\r\n"
                "<TITLE>echo-cpp</TITLE>\n"
                "<H1>echo-cpp</H1>\n"
                "<H4>PID: " << g_pid << "</H4>\n"
                "<H4>Request Number: " << ++g_count << "</H4>\n";

        out << "<H4>Process/Initial Environment</H4>\n";
        penv(environ, out);

        out << "<H4>Fcgi Environment</H4>\n";
        penv(m_request->envp(), out);

        auto val = out.str();

        FCGX_PutStr(val.c_str(), val.size(), m_request->out());
    }
};

class Acceptor {
    asio::io_context &m_io;

public:
    Acceptor(asio::io_context &io) : m_io(io) {}

    void operator() () {
        auto request = FcgiRequest::create();

        if ( !request->accept() ) return;

        m_io.post(Handler(request));

        m_io.post(*this);
    }
};

int main(void) {

    asio::io_context io;

    auto acceptor = Acceptor(io);

    FCGX_Init();

    io.post(acceptor);

    io.run();

    return 0;
}
