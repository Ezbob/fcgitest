#include <stdlib.h>
#ifdef _WIN32
#include <process.h>
#else
#include <unistd.h>
extern char ** environ;
#endif

#include "fcgiapp.h"
#include <iostream>
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

class Acceptor {
    asio::io_context &m_io;
    int m_count = 0;
    long m_pid = 0;

public:
    Acceptor(asio::io_context &io) : m_io(io) {
        m_pid = getpid();
    }

    void operator() () {
        auto sout = std::stringstream();
        auto request = FcgiRequest::create();

        if ( !request->accept() ) return;

        sout << "Content-type: text/html\r\n\r\n"
                "<TITLE>echo-cpp</TITLE>\n"
                "<H1>echo-cpp</H1>\n"
                "<H4>PID: " << m_pid << "</H4>\n"
                "<H4>Request Number: " << ++m_count << "</H4>\n";

        sout << "<H4>Process/Initial Environment</H4>\n";
        penv(environ, sout);

        sout << "<H4>Fcgi Environment</H4>\n";
        penv(request->envp(), sout);

        auto val = sout.str();

        FCGX_PutStr(val.c_str(), val.size(), request->out());

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
