
#include "fcgipp/http/method.hpp"

using namespace fcgipp;

HttpMethod fcgipp::string_to_httpmethod(std::string const& s) {
    if (s == "GET") {
        return HttpMethod::Get;
    } else if (s == "POST") {
        return HttpMethod::Post;
    } else if (s == "PUT") {
        return HttpMethod::Put;
    } else if (s == "PATCH") {
        return HttpMethod::Patch;
    } else if (s == "DELETE") {
        return HttpMethod::Delete;
    } else if (s == "OPTIONS") {
        return HttpMethod::Options;
    } else {
        return HttpMethod::Not_a_method;
    }
}

std::string fcgipp::httpmethod_to_string(fcgipp::HttpMethod const& m) {
    if (m == fcgipp::HttpMethod::Get) {
        return "GET";
    } else if (m == fcgipp::HttpMethod::Post) {
        return "POST";
    } else if (m == fcgipp::HttpMethod::Put) {
        return "PUT";
    } else if (m == fcgipp::HttpMethod::Patch) {
        return "PATCH";
    } else if (m == fcgipp::HttpMethod::Delete) {
        return "DELETE";
    } else if (m == fcgipp::HttpMethod::Options) {
        return "OPTIONS";
    } else {
        return "";
    }
}