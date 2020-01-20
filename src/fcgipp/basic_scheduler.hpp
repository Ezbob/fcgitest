
#pragma once

#include <functional>

namespace fcgipp {

    struct BasicScheduler {
        virtual ~BasicScheduler() = default;
        virtual void schedule_task(std::function<void()> const&) = 0;
    };

}; // namespace fcgipp
