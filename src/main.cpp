#include <stdlib.h>
#ifdef _WIN32
#include <process.h>
#else
#include <unistd.h>
extern char ** environ;
#endif

#include "asio.hpp"
#include "fcgipp/fcgipp.hpp"
#include "fcgipp_shims/asio_multiplexer.hpp"
#include "json11.hpp"

#include <thread>
#include <sstream>
#include <chrono>
#include <ctime>
#include <type_traits>

static long g_pid = getpid();
static int g_count = 0;

class MyHandler : public fcgipp::BasicHandler {

    void penv(const char * const * envp, std::ostream &stream)
    {
        stream << "<pre>" << fcgipp::HTTP_LINE_END;
        for ( ; *envp; ++envp)
        {
            stream << *envp << fcgipp::HTTP_LINE_END;
        }
        stream << "</pre>";
    }

    void penv(const std::vector<const char *> &envp, std::ostream &stream)
    {
        stream << "<pre>" << fcgipp::HTTP_LINE_END;
        for ( auto c : envp )
        {
            stream << c << fcgipp::HTTP_LINE_END;
        }
        stream << "</pre>";
    }

public:
    void handle(std::shared_ptr<fcgipp::BasicServerRequestResponse> rr) {
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

        auto parameters = rr->get_parameters();
        penv(parameters, out);

        out << "</body>"
               "</html>";

        rr->respond_with(resp);
    }
};

struct MyJsonHandler : public fcgipp::BasicHandler {
    void handle(std::shared_ptr<fcgipp::BasicServerRequestResponse> rr) {
        fcgipp::JsonResponse resp;

        auto t = std::time(nullptr);
        char timestr[100];
        std::strftime(timestr, sizeof(timestr), "%c %Z", std::localtime(&t));

        json11::Json j = json11::Json::object {
            {"localtime", std::string(timestr)}
        };

        resp.body() << j.dump();

        rr->respond_with(resp);
    }
};


int main(void) {
    asio::io_context io;
    asio::io_context::work work_io(io);

    fcgipp::FcgiApplication app(std::unique_ptr<fcgipp::BasicMultiplexer>(new fcgipp::AsioMultiplexer(io)));

    auto &dispatcher = app.get_dispatcher();

    dispatcher.add_endpoint("/", fcgipp::HttpMethod::Get, std::make_shared<MyHandler>());
    dispatcher.add_endpoint("/time", fcgipp::HttpMethod::Get, std::make_shared<MyJsonHandler>());

    app.start();

    // handler is running on the first thread
    io.run();

    return 0;
}
