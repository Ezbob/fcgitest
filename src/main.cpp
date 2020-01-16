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
#include "fcgipp_shims/asio_multiplexer.hpp"
#include "json11.hpp"

#include <thread>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <ctime>

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

        rr->answerWith(resp);
    }
};

class MyJsonHandler : public fcgipp::BasicHandler {
public:
    void handle(std::shared_ptr<fcgipp::BasicServerRequestResponse> rr) {
        fcgipp::JsonResponse resp;

        auto t = std::time(nullptr);
        char timestr[100];
        std::strftime(timestr, sizeof(timestr), "%c %Z", std::localtime(&t));

        json11::Json j = json11::Json::object {
            {"localtime", std::string(timestr)}
        };

        resp.body() << j;

        rr->answerWith(resp);
    }
};


int main(void) {
    asio::io_context io;
    asio::io_context::work work_io(io);

    fcgipp::DefaultAuthenticator auth;
    fcgipp::AsioMultiplexer multiplex(io);
    fcgipp::DefaultDispatcher dispatcher(auth, multiplex);
    fcgipp::FcgiAcceptor acceptor(dispatcher);

    auto root_handler = std::make_shared<MyHandler>();
    auto clock_handler = std::make_shared<MyJsonHandler>();

    dispatcher.add_get("/", root_handler);
    dispatcher.add_get("/time", clock_handler);

    std::thread accepting_thread(&fcgipp::FcgiAcceptor::start_accepting, acceptor);

    accepting_thread.detach();

    // handler is running on the first thread
    io.run();

    return 0;
}
