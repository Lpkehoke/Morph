#pragma once

#include "base/taskqueue.h"

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
        Trace,
        Debug,
        Info,
        Warning,
        Error,
        Fatal
    };

    struct LogRecord
    {
        LogRecord() = default;
        ~LogRecord() = default;

        LogRecord(const LogRecord& other);
        LogRecord(LogRecord&& other);

        std::string                             message;
        Severity                                severity;
        std::chrono::system_clock::time_point   timestamp;
    };

    using State = std::vector<LogRecord>;

    Logger()  = default;
    ~Logger() = default;

    void log(Severity severity, const std::string& message);

    State state() const;

  private:
    void post_record_to_queue(LogRecord&& lr);

    State               m_state;
    base::TaskQueue     m_action_queue;
};

} // namespace platform
