#include "platform/logger.h"
#include "base/observable.h"

#include <cstddef>
#include <vector>
#include <string>
#include <thread>
#include <chrono>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using namespace testing;
using namespace platform;
using namespace std;
using namespace std::literals::chrono_literals;

using Severity = Logger::Severity;

TEST(LoggerTest, create_logger)
{
    vector<string> logs_str {
        "Trace",
        "Debug",
        "Info",
        "Warning",
        "Error",
        "Fatal"
    };

    vector<Severity> logs_severity {
        Severity::Trace,
        Severity::Debug,
        Severity::Info,
        Severity::Warning,
        Severity::Error,
        Severity::Fatal
    };
    auto size = logs_str.size();

    Logger logger;
    for(size_t i = 0; i < size; ++i) {
        logger.log(logs_severity[i], logs_str[i]);
    }

    this_thread::sleep_for(1ms);

    auto logReport = logger.state();

    for (size_t i = 0; i < size; ++i) {
        ASSERT_EQ(logReport[i].message, logs_str[i]);
        ASSERT_EQ(logReport[i].severity, logs_severity[i]);
    }
}
