#include "logger.h"

#include <chrono>
#include <string>
#include <utility>

namespace platform
{

using LogRecord  = Logger::LogRecord;
using Severity   = Logger::Severity;

LogRecord::LogRecord(LogRecord&& other)
    : id(std::move(other.id))
    , info(std::move(other.info))
    , severity(std::move(other.severity))
    , time(std::move(other.time))
{}

LogRecord::LogRecord(const LogRecord& other)
    : id(other.id)
    , info(other.info)
    , severity(other.severity)
    , time(other.time)
{}

void Logger::log_trace(const std::string& info)
{
    auto lr = create_log_record(info, Severity::trace);
    this->m_state.emplace_back(std::move(lr));
}

void Logger::log_debug(const std::string& info)
{
    auto lr = create_log_record(info, Severity::debug);
    this->m_state.emplace_back(std::move(lr));
}

void Logger::log_info(const std::string& info)
{
    auto lr = create_log_record(info, Severity::info);
    this->m_state.emplace_back(std::move(lr));
}

void Logger::log_warning(const std::string& info)
{
    auto lr = create_log_record(info, Severity::warning);
    this->m_state.emplace_back(std::move(lr));
}

void Logger::log_error(const std::string& info)
{
    auto lr = create_log_record(info, Severity::error);
    this->m_state.emplace_back(std::move(lr));
}

void Logger::log_fatal(const std::string& info)
{
    auto lr = create_log_record(info, Severity::fatal);
    this->m_state.emplace_back(std::move(lr));
}

Logger::State Logger::get_state() const
{
    return this->m_state;
}

LogRecord Logger::create_log_record(const std::string& info, const Severity severity)
{
    LogRecord lr;

    lr.id   = this->m_current_id++;
    lr.severity = severity;
    lr.info = info;
    lr.time = std::chrono::system_clock::now();

    return lr;
}

} // namespace platform
