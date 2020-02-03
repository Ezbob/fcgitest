
#pragma once

#include "fcgisrv/fcgisrv.hpp"

namespace fcgitest {

    static long g_pid = getpid();
    static int g_count = 0;

    class Echo_Handler: public fcgisrv::IHandler {

      private:
        void penv(const char *const *, std::ostream &);
        void penv(const std::vector<const char *> &, std::ostream &);

      public:
        void handle(std::shared_ptr<fcgisrv::IServer_Request_Response>);
    };

} // namespace fcgisrv
