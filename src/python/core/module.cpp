#include "python/core/bindcore.h"

#include <pybind11/pybind11.h>


PYBIND11_MODULE(_core, m)
{
    bind_logger(m);
    bind_node(m);
    bind_node_factory(m);
    bind_node_storage(m);
    bind_core(m);
    bind_plugin_manager(m);
}
