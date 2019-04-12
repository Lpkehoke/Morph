#include "python/core/bindcore.h"
#include "python/core/pynodefactory.h"

#include "core/nodefactory.h"
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
    py::class_<NodeFactory::NodeKnobsInfo>(scope, "KnobsInfo")
        .def(py::init<>())
        .def(
            "set_input_knob_schema",
            [](
                NodeFactory::NodeKnobsInfo& self,
                const std::string&          knob_name,
                const std::string&          knob_schema_name)
            {
                return self.input_knob_schema[knob_name] = knob_schema_name;
            })
        .def(
            "set_output_knob_schema",
            [](
                NodeFactory::NodeKnobsInfo& self,
                const std::string&          knob_name,
                const std::string&          knob_schema_name)
            {
                return self.output_knob_schema[knob_name] = knob_schema_name;
            });

    py::class_<NodeFactory, PyNodeFactory, NodeFactoryPtr>(scope, "NodeFactory")
        .def(py::init<>())
        .def("create", &NodeFactory::create)
        .def("model", &NodeFactory::model)
        .def("knobs_info", &NodeFactory::knobs_info);
}
