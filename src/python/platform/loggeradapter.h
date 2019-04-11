#pragma once

#include "platform/logger.h"
#include "platform/platformcoretypes.h"

#include <pybind11/pybind11.h>

#include <string>


class LoggerAdapter
{
  public:
    LoggerAdapter(platform::LoggerPtr logger);

    void log(platform::Logger::Severity severity, const std::string& message);

    platform::Logger::State state() const;

    void subscribe(const pybind11::object& on_update);

  private:
    platform::LoggerPtr m_logger;
};
