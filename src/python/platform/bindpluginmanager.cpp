#include "python/platform/bindplatform.h"

#include "platform/platformcoretypes.h"
#include "platform/pluginmanager.h"

#include <pybind11/pybind11.h>

using namespace platform;

namespace py = pybind11;


void bind_plugin_manager(py::handle scope)
{
    py::class_<PluginManager, PluginManagerPtr>(scope, "PluginManager")
        .def(py::init<>());
}
