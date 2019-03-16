#pragma once

#include <chrono>
#include <cstddef>
#include <string>
#include <vector>

namespace platform
{

class Logger
{
  public:
    enum class Severity
    {
        trace,
        debug,
        info,
        warning,
        error,
        fatal
    };

    struct LogRecord
    {
        LogRecord() = default;
        ~LogRecord() = default;

        LogRecord(LogRecord&& other);
        LogRecord(const LogRecord& other);

        std::size_t                                        id;
        std::string                                        info;
        Severity                                           severity;
        std::chrono::time_point<std::chrono::system_clock> time;

    };

    using State = std::vector<LogRecord>;

    Logger()  = default;
    ~Logger() = default;

    void log_trace(const std::string& info);
    void log_debug(const std::string& info);
    void log_info(const std::string& info);
    void log_warning(const std::string& info);
    void log_error(const std::string& info);
    void log_fatal(const std::string& info);

    State get_state() const;

  private:
    LogRecord create_log_record(const std::string& info, const Severity severity);

    State       m_state;
    std::size_t m_current_id;
};

} // namespace platform
