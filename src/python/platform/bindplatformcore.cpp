#include "python/platform/bindplatform.h"

#include "python/platform/nodestorageadapter.h"

#include "platform/platformcore.h"
#include "platform/platformcoretypes.h"

#include <pybind11/pybind11.h>

using namespace platform;

namespace py = pybind11;


void bind_platform_core(py::handle scope)
{
    py::class_<PlatformCore>(scope, "PlatformCore")
        .def(py::init<>())
        .def(
            "node_storage",
            [](const PlatformCore& platform_core)
            {
                return NodeStorageAdapter(platform_core.node_storage());
            })
        .def("plugin_manager", &PlatformCore::plugin_manager)
        .def("logger", &PlatformCore::logger);
}
