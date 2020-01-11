
#ifndef _HEADER_FILE_http_method_20200111123649_
#define _HEADER_FILE_http_method_20200111123649_

namespace fcgipp {
    enum class HttpMethod {
        GET,
        POST,
        PUT,
        PATCH,
        DELETE,
        OPTIONS,
        ANY_METHOD,
        NOT_A_METHOD
    };

    HttpMethod string_to_httpmethod(std::string &&s) {
        if (s == "GET") {
            return HttpMethod::GET;
        } else if (s == "POST") {
            return HttpMethod::POST;
        } else if (s == "PUT") {
            return HttpMethod::PUT;
        } else if (s == "PATCH") {
            return HttpMethod::PATCH;
        } else if (s == "DELETE") {
            return HttpMethod::DELETE;
        } else if (s == "OPTIONS") {
            return HttpMethod::OPTIONS;
        } else {
            return HttpMethod::NOT_A_METHOD;
        }
    }

    std::string httpmethod_to_string(HttpMethod &&m) {
        if (m == HttpMethod::GET) {
            return "GET";
        } else if (m == HttpMethod::POST) {
            return "POST";
        } else if (m == HttpMethod::PUT) {
            return "PUT";
        } else if (m == HttpMethod::PATCH) {
            return "PATCH";
        } else if (m == HttpMethod::DELETE) {
            return "DELETE";
        } else if (m == HttpMethod::OPTIONS) {
            return "OPTIONS";
        } else {
            return "";
        }
    }
};

#endif