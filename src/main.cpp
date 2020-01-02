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

int main (void)
{
    int count = 0;
    long pid = getpid();

    FCGX_Init();

    while (true) {
        auto sout = std::stringstream();
        auto request = FcgiRequest::create();

        if ( !request->accept() ) break;

        sout << "Content-type: text/html\r\n\r\n"
                "<TITLE>echo-cpp</TITLE>\n"
                "<H1>echo-cpp</H1>\n"
                "<H4>PID: " << pid << "</H4>\n"
                "<H4>Request Number: " << ++count << "</H4>\n";

        sout << "<H4>Process/Initial Environment</H4>\n";
        penv(environ, sout);

        sout << "<H4>Fcgi Environment</H4>\n";
        penv(request->envp(), sout);

        auto val = sout.str();

        FCGX_PutStr(val.c_str(), val.size(), request->out());
    }

    return 0;
}
