#include "python/platform/bindplatform.h"

#include <pybind11/pybind11.h>


PYBIND11_MODULE(_platform, m)
{
    bind_logger(m);
    bind_node_storage(m);
    bind_platform_core(m);
    bind_plugin_manager(m);
}
