
#pragma once

namespace fcgipp {

    struct BasicAcceptor {
        virtual ~BasicAcceptor() = default;
        virtual void start() = 0;
    };

}; // namespace fcgipp
