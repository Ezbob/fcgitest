
#ifndef _HEADER_FILE_response_20200113210402_
#define _HEADER_FILE_response_20200113210402_

#include <string>
#include <sstream>
#include <ostream>
#include "fcgipp/handler_response.hpp"

namespace fcgipp {

    static const std::string HTTP_LINE_END = "\r\n";

    class HttpResponse : public fcgipp::BasicResponse {
    protected:
        std::stringstream m_header;
        std::stringstream m_body;
        int m_error_code = 200;

        std::string translate_code();

    public:

        void put_header(std::string const& raw) {
            m_header << (raw + HTTP_LINE_END);
        }

        void put_header(std::string const& name, std::string const& value) {
            m_header << (name + ": " + value + HTTP_LINE_END);
        }

        void status(int code) {
            m_error_code = code;
        }

        int status() {
            return m_error_code;
        }

        std::ostream &body() {
            return m_body;
        }

        std::string render() override;
    };

    class JsonResponse : public HttpResponse {
        std::string render() override;
    };

};

#endif