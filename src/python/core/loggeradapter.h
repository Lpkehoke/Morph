#pragma once

#include "core/logger.h"
#include "core/coretypes.h"

#include <pybind11/pybind11.h>

#include <string>


class LoggerAdapter
{
  public:
    LoggerAdapter(core::LoggerPtr logger);

    void log(core::Logger::Severity severity, const std::string& message);

    core::Logger::State state() const;

    void subscribe(const pybind11::object& on_update);

  private:
    core::LoggerPtr m_logger;
};
