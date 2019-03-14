#include "logger.h"

#include <chrono>
#include <string>

namespace platform
{

using LogObject = Logger::LogObject;
using TypeLog   = Logger::TypeLog;

LogObject::LogObject(LogObject&& log)
    : id(std::move(log.id))
    , info(std::move(log.info))
    , view(std::move(log.view))
    , time(std::move(log.time))
{}

LogObject::LogObject(const LogObject& log)
    : id(log.id)
    , info(log.info)
    , view(log.view)
    , time(log.time)
{}

LogObject& LogObject::operator=(LogObject&& log) {
    id   = std::move(log.id);
    info = std::move(log.info);
    view = std::move(log.view);
    time = std::move(log.time);

    return *this;
}

void Logger::log_trace(const std::string& info)
{
    auto log = create_log_object(info, TypeLog::trace);
    this->m_state.emplace_back(std::move(log));
}

void Logger::log_debug(const std::string& info)
{
    auto log = create_log_object(info, TypeLog::debug);
    this->m_state.emplace_back(std::move(log));
}

void Logger::log_info(const std::string& info)
{
    auto log = create_log_object(info, TypeLog::info);
    this->m_state.emplace_back(std::move(log));
}

void Logger::log_warning(const std::string& info)
{
    auto log = create_log_object(info, TypeLog::warning);
    this->m_state.emplace_back(std::move(log));
}

void Logger::log_error(const std::string& info)
{
    auto log = create_log_object(info, TypeLog::error);
    this->m_state.emplace_back(std::move(log));
}

void Logger::log_fatal(const std::string& info)
{
    auto log = create_log_object(info, TypeLog::fatal);
    this->m_state.emplace_back(std::move(log));
}

Logger::StateType Logger::get_state() const
{
    return this->m_state;
}

LogObject Logger::create_log_object(const std::string& info, const TypeLog view)
{
    LogObject log;

    log.id   = this->m_current_id++;
    log.view = view;
    log.info = info;
    log.time = std::chrono::system_clock::now();

    return log;
}

} // namespace platform
