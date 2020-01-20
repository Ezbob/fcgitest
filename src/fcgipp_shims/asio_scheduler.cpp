
#include "fcgipp_shims/asio_scheduler.hpp"

using namespace fcgipp;

void fcgipp::AsioScheduler::schedule_task(std::function<void()> const& task) {
    m_io_ctx.post(task);
}
