#include "logger.h"

#include <chrono>
#include <string>
#include <utility>

namespace platform
{

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

    this->post_record_to_queue(std::move(lr));
}

Logger::State Logger::state() const
{
    return this->m_state;
}

void Logger::post_record_to_queue(LogRecord&& lr) {
    this->m_action_queue.post([=](){
        this->m_state.emplace_back(std::move(lr));
    });
}

} // namespace platform
