
#pragma once

#include "fcgipp/basic_scheduler.hpp"
#include "asio.hpp"

namespace fcgipp {

    class AsioScheduler : public BasicScheduler {
        asio::io_context &m_io_ctx;

    public:
        AsioScheduler(asio::io_context &io_ctx) : m_io_ctx(io_ctx) {}

        void schedule_task(std::function<void()> const& task) override;
    };

}; // namespace fcgipp
