
#pragma once

#include "fcgisrv/fcgisrv.hpp"
#include "json11.hpp"
#include <ctime>

namespace fcgitest {

    class Current_Time_Handler: public fcgisrv::IHandler {
      public:
        void
            handle(std::shared_ptr<fcgisrv::IServer_Request_Response>) override;
    };

}; // namespace fcgitest
