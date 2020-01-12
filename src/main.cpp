#include <stdlib.h>
#ifdef _WIN32
#include <process.h>
#else
#include <unistd.h>
extern char ** environ;
#endif

#include "asio.hpp"
#include "fcgipp/fcgi.hpp"
#include "fcgipp_shims/json11_shim.hpp"
#include "json11.hpp"

#include <sstream>
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
        fcgipp::JsonResponse resp;

        auto t = std::time(nullptr);
        char timestr[100];
        std::strftime(timestr, sizeof(timestr), "%c %Z", std::localtime(&t));

        json11::Json j = json11::Json::object {
            {"localtime", std::string(timestr)}
        };

        resp.body() << j;

        fcgir->answerWith(resp);
    }
};


int main(void) {
    asio::io_context io;

    auto server = fcgipp::make_server(io);
    auto &dispatcher = server.get_dispatcher();

    auto root_handler = fcgipp::make_handler<MyHandler>();
    auto clock_handler = fcgipp::make_handler<MyJsonHandler>();

    dispatcher.add_endpoint("/", root_handler);
    dispatcher.add_endpoint("/time", clock_handler);

    io.post(server);

    io.run();

    return 0;
}
