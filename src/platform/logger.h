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
    enum class TypeLog
    {
        trace,
        debug,
        info,
        warning,
        error,
        fatal
    };

    struct LogObject
    {
        LogObject() = default;
        ~LogObject() = default;

        LogObject(LogObject&& log);
        LogObject(const LogObject& log);
        LogObject& operator=(LogObject&& log);

        std::size_t                                        id;
        std::string                                        info;
        TypeLog                                            view;
        std::chrono::time_point<std::chrono::system_clock> time;

    };

    using StateType = std::vector<LogObject>;

    Logger()  = default;
    ~Logger() = default;

    void log_trace(const std::string& info);
    void log_debug(const std::string& info);
    void log_info(const std::string& info);
    void log_warning(const std::string& info);
    void log_error(const std::string& info);
    void log_fatal(const std::string& info);

    StateType get_state() const;

  private:
    LogObject create_log_object(const std::string& info, const TypeLog view);

    StateType   m_state;
    std::size_t m_current_id;
};

} // namespace platform
