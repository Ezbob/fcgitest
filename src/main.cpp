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
#include <memory>

using namespace std;

// Maximum number of bytes allowed to be read from stdin
static const unsigned long STDIN_MAX = 1000000;

static void penv(const char * const * envp, std::stringstream &stream)
{
    stream << "<PRE>\n";
    for ( ; *envp; ++envp)
    {
        stream << *envp << "\n";
    }
    stream << "</PRE>\n";
}

class FRequest {
    FCGX_Request m_request;
    bool is_accepted;
public:
    FRequest() : is_accepted(false) {
        FCGX_InitRequest(&m_request, 0, 0);
    }

    ~FRequest() {
        if (is_accepted) FCGX_Finish_r(&m_request);
    }

    operator FCGX_Request &() {
        return m_request;
    }

    static std::shared_ptr<FRequest> create();

    bool accept() {
        is_accepted = (FCGX_Accept_r(&m_request) == 0);
        return is_accepted;
    }

    FCGX_Stream *out() {
        return m_request.out;
    }

    FCGX_Stream *err() {
        return m_request.err;
    }

    FCGX_Stream *in() {
        return m_request.in;
    }
};

std::shared_ptr<FRequest> FRequest::create() {
    return std::make_shared<FRequest>();
}

int main (void)
{
    int count = 0;
    long pid = getpid();

    auto sout = std::stringstream();

    FCGX_Init();

    while (1) {
        auto request = FRequest::create();

        if ( !request->accept() ) break;

        sout << "Content-type: text/html\r\n\r\n"
                "<TITLE>echo-cpp</TITLE>\n"
                "<H1>echo-cpp</H1>\n"
                "<H4>PID: " << pid << "</H4>\n"
                "<H4>Request Number: " << ++count << "</H4>\n";

        sout << "<H4>Process/Initial Environment</H4>\n";
        penv(environ, sout);

        auto val = sout.str();

        FCGX_PutStr(val.c_str(), val.size(), request->out());

        sout.clear();
    }

    return 0;
}
