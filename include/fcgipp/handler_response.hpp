
#ifndef _HEADER_FILE_handler_response_20200110190205_
#define _HEADER_FILE_handler_response_20200110190205_

#include <ostream>
#include <string>
#include <sstream>

namespace fcgipp {

    class BasicResponse {
    public:
        virtual std::string render() = 0;
    };

    static const std::string HTTP_LINE_END = "\r\n";

    class HttpResponse : public BasicResponse {
        std::stringstream m_header;
        std::stringstream m_body;

    public:

        void put_header(std::string const& raw) {
            m_header << (raw + HTTP_LINE_END);
        }

        void put_header(std::string const& name, std::string const& value) {
            m_header << (name + ": " + value + HTTP_LINE_END);
        }

        std::ostream &body() {
            return m_body;
        }

        std::string render() override {
            std::stringstream res;

            res << m_header.str();
            res << HTTP_LINE_END + HTTP_LINE_END;
            res << m_body.str();

            return res.str();
        }
    };

};

#endif