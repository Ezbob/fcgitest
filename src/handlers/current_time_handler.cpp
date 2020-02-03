#include "handlers/current_time_handler.hpp"

#include <memory>

using namespace fcgitest;

void Current_Time_Handler::handle(
    std::shared_ptr<fcgisrv::IServer_Request_Response> rr) {

    fcgisrv::Json_Response json_resp;

    auto t = std::time(nullptr);
    char timestr[100];
    std::strftime(timestr, sizeof(timestr), "%c %Z", std::localtime(&t));

    json11::Json j = json11::Json::object{{"localtime", std::string(timestr)}};

    json_resp.body() << j.dump();

    rr->respond_with(json_resp);
}