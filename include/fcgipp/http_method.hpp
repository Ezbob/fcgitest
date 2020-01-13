
#ifndef _HEADER_FILE_http_method_20200111123649_
#define _HEADER_FILE_http_method_20200111123649_

namespace fcgipp {
    enum class HttpMethod {
        Get,
        Post,
        Put,
        Patch,
        Delete,
        Options,
        Not_a_method
    };

    HttpMethod string_to_httpmethod(std::string &&s) {
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

    std::string httpmethod_to_string(fcgipp::HttpMethod &&m) {
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
};

#endif