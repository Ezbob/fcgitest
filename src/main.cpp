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
#include <chrono>
#include <iomanip>
#include <ctime>

static void penv(const char * const * envp, std::ostream &stream)
{
    stream << "<pre>" << fcgipp::HTTP_LINE_END;
    for ( ; *envp; ++envp)
    {
        stream << *envp << fcgipp::HTTP_LINE_END;
    }
    stream << "</pre>";
}

static long g_pid = getpid();
static int g_count = 0;

class MyHandler : public fcgipp::BasicHandler {
public:
    void handle(std::shared_ptr<fcgipp::FcgiReqRes> fcgir) {
        fcgipp::HttpResponse resp;
        std::ostream &out = resp.body();

        resp.put_header("Content-type", "text/html");

        out <<  "<html>";
        out <<  "<head>"
                    "<title>echo-cpp</title>"
                    "<meta charset=\"UTF-8\">"
                "</head>";

        out <<  "<body>"
                "<h1>echo-cpp</h1>"
                "<h4>PID: " << g_pid << "</h4>"
                "<h4>Request Number: " << ++g_count << "</h4>"
                "<h4>Process/Initial Environment</h4>";

        penv(environ, out);

        out <<  "<h4>Fcgi Environment</h4>";
        penv(fcgir->envp(), out);

        out << "</body>"
               "</html>";

        fcgir->answerWith(resp);
    }
};

class MyJsonHandler : public fcgipp::BasicHandler {
public:
    void handle(std::shared_ptr<fcgipp::FcgiReqRes> fcgir) {
        fcgipp::HttpResponse resp;
        std::ostream &out = resp.body();
        auto t = std::time(nullptr);

        out << "{"
            << "\"localtime\": \"" << std::put_time(std::localtime(&t), "%c %Z") << "\""
            << "}";

        fcgir->answerWith(resp);
    }
};


int main(void) {

    if ( FCGX_Init() != 0 ) return 1;

    asio::io_context io;

    auto dispatcher = fcgipp::UriDispatcher<asio::io_context>(io);

    auto root_handler = fcgipp::make_handler<MyHandler>();
    auto clock_handler = fcgipp::make_handler<MyJsonHandler>();

    dispatcher.add_endpoint("/", root_handler);
    dispatcher.add_endpoint("/time", clock_handler);

    auto server = fcgipp::make_server(io, dispatcher);

    io.post(server);

    io.run();

    return 0;
}
