
#ifndef _HEADER_FILE_handler_response_20200110190205_
#define _HEADER_FILE_handler_response_20200110190205_

#include <ostream>
#include <string>
#include <sstream>

namespace fcgipp {

    struct BasicResponse {
        virtual ~BasicResponse() = default;
        virtual std::string render() = 0;
    };

    static const std::string HTTP_LINE_END = "\r\n";

    class HttpResponse : public BasicResponse {
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

    std::ostream &operator <<(std::ostream &os, BasicResponse &);
};

#endif