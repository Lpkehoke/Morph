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

NodePtr PyNodeFactory::create(
    core::Node::KnobMap input_knobs,
    core::Node::KnobMap output_knobs) const
{
    PYBIND11_OVERLOAD_PURE(
        NodePtr,
        NodeFactory,
        create,
        input_knobs,
        output_knobs);
}

Dict PyNodeFactory::params() const
{
    PYBIND11_OVERLOAD_PURE(
        Dict,
        NodeFactory,
        params);
}

void bind_node_factory(py::handle scope)
{
    py::class_<NodeFactory, PyNodeFactory, NodeFactoryPtr>(scope, "NodeFactory")
        .def(py::init<>())
        .def("create", &NodeFactory::create)
        .def("params", &NodeFactory::params);
}
