#include "python/core/bindcore.h"

#include "python/core/loggeradapter.h"
#include "python/core/nodestorageadapter.h"

#include "core/core.h"
#include "core/coretypes.h"

#include <pybind11/pybind11.h>

using namespace core;

namespace py = pybind11;


void bind_core(py::handle scope)
{
    py::class_<Core>(scope, "Core")
        .def(py::init<>())
        .def(
            "node_storage",
            [](const Core& core_core)
            {
                return NodeStorageAdapter(core_core.node_storage());
            })
        .def("plugin_manager", &Core::plugin_manager)
        .def(
            "logger",
            [](const Core& core_core)
            {
                return LoggerAdapter(core_core.logger());
            });
}
