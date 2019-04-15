#include "logger.h"

#include "foundation/taskqueue.h"

#include <tbb/spin_mutex.h>

#include <chrono>
#include <string>
#include <utility>

using namespace foundation;

namespace core
{

struct Logger::Impl
{
    Impl()
      : m_task_queue(TaskQueue::Priority::Low)
    {}

    State           m_state;
    TaskQueue       m_task_queue;
    tbb::spin_mutex m_state_mutex;
};

Logger::Logger()
    : impl(new Impl())
{}

Logger::~Logger()
{
    delete impl;
}

Logger::LogRecord::LogRecord(LogRecord&& other)
    : message(std::move(other.message))
    , severity(std::move(other.severity))
    , timestamp(std::move(other.timestamp))
{}

Logger::LogRecord::LogRecord(const LogRecord& other)
    : message(other.message)
    , severity(other.severity)
    , timestamp(other.timestamp)
{}

void Logger::log(Severity severity, const std::string& message)
{
    LogRecord lr;

    lr.severity = severity;
    lr.message = message;
    lr.timestamp = std::chrono::system_clock::now();

    post_record_to_queue(std::move(lr));
}

Logger::State Logger::state() const
{
    tbb::spin_mutex::scoped_lock lock(impl->m_state_mutex);
    return impl->m_state;
}

void Logger::post_record_to_queue(LogRecord&& lr)
{
    impl->m_task_queue.post([=]()
    {
        tbb::spin_mutex::scoped_lock lock(impl->m_state_mutex);
        impl->m_state.emplace_back(std::move(lr));

        lock.release();
        notify();
    });
}

} // namespace core
