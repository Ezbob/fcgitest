
#include "fcgipp_shims/asio_multiplexer.hpp"

using namespace fcgipp;

void fcgipp::AsioMultiplexer::schedule_task(std::function<void()> const& task) {
    m_io_ctx.post(task);
}
