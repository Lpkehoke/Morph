#include "python/platform/bindplatform.h"

#include "platform/logger.h"
#include "platform/platformcoretypes.h"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <pybind11/chrono.h>

namespace py = pybind11;
using namespace platform;

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

    py::class_<Logger, LoggerPtr>(scope, "Logger")
        .def(py::init<>())
        .def("log", &Logger::log)
        .def("state", &Logger::state);
}
