
#pragma once

#include "fcgipp/basic_multiplexer.hpp"
#include "asio.hpp"

namespace fcgipp {

    class AsioMultiplexer : public BasicMultiplexer {
        asio::io_context &m_io_ctx;

    public:
        AsioMultiplexer(asio::io_context &io_ctx) : m_io_ctx(io_ctx) {}

        void schedule_task(std::function<void()> const& task) override;
    };

}; // namespace fcgipp
