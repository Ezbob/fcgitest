
#pragma once

#include <functional>

namespace fcgipp {

    struct BasicMultiplexer {
        virtual ~BasicMultiplexer() = default;
        virtual void schedule_task(std::function<void()> const&) = 0;
    };

}; // namespace fcgipp
