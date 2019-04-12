#include "python/core/bindcore.h"
#include "python/core/loggeradapter.h"

#include "core/logger.h"
#include "core/coretypes.h"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/chrono.h>

#include <utility>

namespace py = pybind11;
using namespace core;


LoggerAdapter::LoggerAdapter(core::LoggerPtr logger)
  : m_logger(std::move(logger))
{}

void LoggerAdapter::log(Logger::Severity severity, const std::string& message)
{
    m_logger->log(std::move(severity), message);
}

Logger::State LoggerAdapter::state() const
{
    return m_logger->state();
}

void LoggerAdapter::subscribe(const pybind11::object& on_update)
{
    m_logger->subscribe(
        [on_update]()
        {
            py::gil_scoped_acquire gil;

            try
            {
                on_update();
            }
            catch(const py::error_already_set&)
            {
                // TODO: supply exception info here.
                throw std::runtime_error("Python exception occured during callback evaluation.");
            }
        });
}

void bind_logger(py::handle scope)
{
    py::enum_<Logger::Severity>(scope, "Severity", py::arithmetic())
        .value("Debug", Logger::Severity::Debug)
        .value("Trace", Logger::Severity::Trace)
        .value("Info", Logger::Severity::Info)
        .value("Warning", Logger::Severity::Warning)
        .value("Error", Logger::Severity::Error)
        .value("Fatal", Logger::Severity::Fatal);

    py::class_<Logger::LogRecord>(scope, "LogRecord")
        .def_property_readonly(
            "timestamp",
            [](const Logger::LogRecord& lr)
            {
                return lr.timestamp;
            })
        .def_property_readonly(
            "severity",
            [](const Logger::LogRecord& lr)
            {
                return lr.severity;
            })
        .def_property_readonly(
            "message",
            [](const Logger::LogRecord& lr)
            {
                return lr.message;
            });

    py::class_<LoggerAdapter>(scope, "Logger")
        .def(py::init<LoggerPtr>())
        .def("log", &LoggerAdapter::log)
        .def("state", &LoggerAdapter::state)
        .def("subscribe", &LoggerAdapter::subscribe);
}
