#pragma once

#include "foundation/observable.h"

#include <chrono>
#include <string>
#include <vector>


namespace core
{

class Logger final: public foundation::Observable
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

    Logger();
    ~Logger();

    void log(Severity severity, const std::string& message);

    State state() const;

  private:
    void post_record_to_queue(LogRecord&& lr);

    struct Impl;
    Impl* impl;
};

} // namespace core
