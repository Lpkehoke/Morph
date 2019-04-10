#include "python/platform/bindplatform.h"
#include "python/platform/nodestorageadaptor.h"
#include "python/platform/pythondictconversion.h"

#include "python/base/bindimmutablemap.h"

#include "platform/attribute.h"
#include "platform/knob.h"
#include "platform/logger.h"
#include "platform/nodestorage.h"
#include "platform/platformcoretypes.h"
#include "platform/pluginmanager.h"

#include <pybind11/pybind11.h>

using namespace platform;

namespace py = pybind11;


//
//  NodeStorageAdaptor implementation.
//

NodeStorageAdaptor::NodeStorageAdaptor(NodeStoragePtr node_storage)
    : m_node_storage(std::move(node_storage))
{}

void NodeStorageAdaptor::dispatch(const py::dict& action)
{
    std::string action_type = py::str(action["type"]);

    if (action_type == "CreateNode")
    {
        auto create_node = from_dict<CreateNode>(action);
        m_node_storage->dispatch(std::move(create_node));
    }
    else if (action_type == "RemoveNode")
    {
        auto remove_node = from_dict<RemoveNode>(action);
        m_node_storage->dispatch(std::move(remove_node));
    }
    else if (action_type == "UpdateNodeMetadata")
    {
        auto update_node_metadata = from_dict<UpdateNodeMetadata>(action);
        m_node_storage->dispatch(std::move(update_node_metadata));
    }
    else if (action_type == "MakeConnection")
    {
        auto make_connection = from_dict<MakeConnection>(action);
        m_node_storage->dispatch(std::move(make_connection));
    }
    else
    {
        throw std::runtime_error("Unsupported action type.");
    }
}

void NodeStorageAdaptor::subscribe(const py::object& on_update)
{
    m_node_storage->subscribe(
        [on_update]()
        {
            py::gil_scoped_acquire gil;

            try
            {
                on_update();
            }
            catch(const py::error_already_set&)
            {
                // TODO: supply exception info here.
                throw std::runtime_error("Python exception occured during callback evaluation.");
            }
        });
}

NodeStorageState NodeStorageAdaptor::state()
{
    return m_node_storage->state();
}

void bind_node_storage(py::handle scope)
{
    ImmutableMap<NodeCollection>(scope, "NodeCollection");
    ImmutableMap<KnobCollection>(scope, "KnobKollection");
    ImmutableMap<AttributeCollection>(scope, "AttributeCollection");
    ImmutableMap<MetadataCollection>(scope, "MetadataCollection");
    ImmutableMap<Metadata>(scope, "Metadata");

    py::class_<Attribute, AttrPtr>(scope, "Attribute");
    py::class_<Knob, KnobPtr>(scope, "Knob");
    
    ImmutableMap<Node::KnobMap>(scope, "KnobMap");
    py::class_<Node, NodePtr>(scope, "Node")
        .def("input_knobs", &Node::input_knobs)
        .def("output_knobs", &Node::output_knobs);

    py::class_<NodeStorageState>(scope, "NodeStorageState")
        .def_property_readonly(
            "nodes",
            [](const NodeStorageState& state)
            {
                return state.m_nodes;
            })
        .def_property_readonly(
            "knobs",
            [](const NodeStorageState& state)
            {
                return state.m_knobs;
            })
        .def_property_readonly(
            "attributes",
            [](const NodeStorageState& state)
            {
                return state.m_attributes;
            })
        .def_property_readonly(
            "node_metadata",
            [](const NodeStorageState& state)
            {
                return state.m_node_metadata;
            });
    
    py::class_<NodeStorageAdaptor>(scope, "NodeStorage")
        .def("dispatch", &NodeStorageAdaptor::dispatch)
        .def("state", &NodeStorageAdaptor::state)
        .def("subscribe", &NodeStorageAdaptor::subscribe);
}
