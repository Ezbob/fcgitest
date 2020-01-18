
#pragma once

namespace fcgipp {

    struct BasicAcceptor {
        virtual ~BasicAcceptor() = default;
        virtual void start_accepting() = 0;
    };

}; // namespace fcgipp
