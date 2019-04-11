#include "python/platform/bindplatform.h"
#include "python/platform/pynodefactory.h"

#include "platform/nodefactory.h"

#include <pybind11/pybind11.h>

namespace py = pybind11;

using namespace platform;


//
//  PyNodeFacory implementation.
//

NodePtr PyNodeFactory::create(
    platform::Node::KnobMap input_knobs,
    platform::Node::KnobMap output_knobs) const
{
    PYBIND11_OVERLOAD_PURE(
        NodePtr,
        NodeFactory,
        create,
        input_knobs,
        output_knobs);
}

const std::string& PyNodeFactory::model() const
{
    PYBIND11_OVERLOAD_PURE(
        const std::string&,
        NodeFactory,
        model);
}

const PyNodeFactory::NodeKnobsInfo& PyNodeFactory::knobs_info() const
{
    PYBIND11_OVERLOAD_PURE(
        const NodeKnobsInfo&,
        NodeFactory,
        knobs_info);
}

void bind_node_factory(py::handle scope)
{
    py::class_<NodeFactory, PyNodeFactory>(scope, "NodeFactory")
        .def(py::init<>())
        .def("create", &NodeFactory::create)
        .def("model", &NodeFactory::model)
        .def("knobs_info", &NodeFactory::knobs_info);
}
