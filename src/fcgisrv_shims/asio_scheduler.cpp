
#include "fcgisrv_shims/asio_scheduler.hpp"

using namespace fcgisrv;

void fcgisrv::Asio_Scheduler::schedule_task(std::function<void()> const &task) {
    m_io_ctx.post(task);
}
