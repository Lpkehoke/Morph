#include "python/core/bindcore.h"
#include "python/core/pynodefactory.h"

#include "core/node.h"
#include "core/nodefactory.h"
#include "core/nodestoragetypes.h"
#include "core/pluginmanager.h"

#include <pybind11/pybind11.h>

namespace py = pybind11;

using namespace core;


//
//  PyNodeFacory implementation.
//

NodePtr PyNodeFactory::create(const Dict& params) const
{
    PYBIND11_OVERLOAD_PURE(
        NodePtr,
        NodeFactory,
        create,
        params);
}

Dict PyNodeFactory::node_info() const
{
    PYBIND11_OVERLOAD_PURE(
        Dict,
        NodeFactory,
        node_info);
}

void bind_node_factory(py::handle scope)
{
    py::class_<NodeFactory, PyNodeFactory, NodeFactoryPtr>(scope, "NodeFactory")
        .def(py::init<>())
        .def("create", &NodeFactory::create)
        .def("node_info", &NodeFactory::node_info);
}
