#include "python/core/bindcore.h"

#include "core/nodefactory.h"
#include "core/coretypes.h"
#include "core/pluginmanager.h"

#include <pybind11/pybind11.h>

using namespace core;

namespace py = pybind11;


void bind_plugin_manager(py::handle scope)
{
    py::class_<PluginManager, PluginManagerPtr>(scope, "PluginManager")
        .def(py::init<>())
        .def("register_node_factory", &PluginManager::register_node_factory);
}
