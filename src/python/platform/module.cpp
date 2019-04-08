#include "pythondictconversion.inl"

#include "python/base/bindimmutablemap.h"

#include "platform/attribute.h"
#include "platform/knob.h"
#include "platform/logger.h"
#include "platform/node.h"
#include "platform/nodefactory.h"
#include "platform/nodefactoryregistry.h"
#include "platform/nodestorage.h"
#include "platform/nodestoragetypes.h"

#include <pybind11/pybind11.h>
#include <pybind11/chrono.h>
#include <pybind11/stl.h>

#include <chrono>
#include <stdexcept>
#include <string>
#include <utility>

namespace py = pybind11;

namespace platform { struct CreateNode; }
namespace platform { struct MakeConnection; }
namespace platform { struct RemoveNode; }
namespace platform { struct UpdateNodeMetadata; }

using namespace platform;


//
//  Bind Node.
//

void bind_node(const py::handle& m)
{
    bind_map<Node::KnobMap>(m, "KnobMap");

    py::class_<Node, NodePtr>(m, "Node")
        .def("input_knobs", &Node::input_knobs)
        .def("output_knobs", &Node::output_knobs);
}

void bind_node_collection(const py::handle& m)
{
    bind_map<NodeCollection>(m, "NodeCollection");
}


//
//  Bind Knob.
//

void bind_knob(const py::handle& m)
{
    py::class_<Knob, KnobPtr>(m, "Knob");
}

void bind_knob_collection(const py::handle& m)
{
    bind_map<KnobCollection>(m, "KnobKollection");
}


//
//  Bind Attribute.
//

void bind_attribute(const py::handle& m)
{
    py::class_<Attribute, AttrPtr>(m, "Attribute");
}

void bind_attr_collection(const py::handle& m)
{
    bind_map<AttributeCollection>(m, "AttributeCollection");
}

//
//  Bind Metadata.
//

void bind_metadata(const py::handle& m)
{
    bind_map<Metadata>(m, "Metadata");
}

void bind_metadata_collection(const py::handle& m)
{
    bind_map<MetadataCollection>(m, "MetadataCollection");
}


//
//  Bind NodeStorage.
//

namespace
{

class NodeStorageAdaptor : public NodeStorage
{
  public:
    NodeStorageAdaptor(
        NodeFactoryRegistry*    node_factory_registry,
        Logger*                 logger);

    void dispatch(const py::dict& action);
    void subscribe(const py::object& on_update);
};


//
//  NodeStorageAdaptor implementation.
//

NodeStorageAdaptor::NodeStorageAdaptor(
    NodeFactoryRegistry*    node_factory_registry,
    Logger*                 logger)
    : NodeStorage(node_factory_registry, logger)
{}

void NodeStorageAdaptor::dispatch(const py::dict& action)
{
    std::string action_type = py::str(action["type"]);

    if (action_type == "CreateNode")
    {
        auto create_node = from_dict<CreateNode>(action);
        NodeStorage::dispatch(std::move(create_node));
    }
    else if (action_type == "RemoveNode")
    {
        auto remove_node = from_dict<RemoveNode>(action);
        NodeStorage::dispatch(std::move(remove_node));
    }
    else if (action_type == "UpdateNodeMetadata")
    {
        auto update_node_metadata = from_dict<UpdateNodeMetadata>(action);
        NodeStorage::dispatch(std::move(update_node_metadata));
    }
    else if (action_type == "MakeConnection")
    {
        auto make_connection = from_dict<MakeConnection>(action);
        NodeStorage::dispatch(std::move(make_connection));
    }
    else
    {
        throw std::runtime_error("Unsupported action type.");
    }
}

void NodeStorageAdaptor::subscribe(const py::object& on_update)
{
    NodeStorage::subscribe(
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

} // namespace

void bind_node_storage(const py::handle& m)
{
    py::class_<NodeStorageState>(m, "NodeStorageState")
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

    py::class_<NodeFactoryRegistry>(m, "NodeFactoryRegistry")
        .def(py::init<>());
    
    py::enum_<Logger::Severity>(m, "Severity", py::arithmetic())
        .value("Debug", Logger::Severity::Debug)
        .value("Trace", Logger::Severity::Trace)
        .value("Info", Logger::Severity::Info)
        .value("Warning", Logger::Severity::Warning)
        .value("Error", Logger::Severity::Error)
        .value("Fatal", Logger::Severity::Fatal);

    py::class_<Logger::LogRecord>(m, "LogRecord")
        .def_property_readonly(
            "timestamp",
            [](const Logger::LogRecord& lr)
            {
                return lr.timestamp;
            })
        .def_property_readonly(
            "severity",
            [](const Logger::LogRecord& lr)
            {
                return lr.severity;
            })
        .def_property_readonly(
            "message",
            [](const Logger::LogRecord& lr)
            {
                return lr.message;
            });

    py::class_<Logger>(m, "Logger")
        .def(py::init<>())
        .def("log", &Logger::log)
        .def("state", &Logger::state);

    py::class_<NodeStorageAdaptor>(m, "NodeStorage")
        .def(py::init<NodeFactoryRegistry*, Logger*>())
        .def("dispatch", &NodeStorageAdaptor::dispatch)
        .def("state", &NodeStorageAdaptor::state)
        .def("subscribe", &NodeStorageAdaptor::subscribe);

}

PYBIND11_MODULE(_platform, m)
{
    bind_node(m);
    bind_node_collection(m);
    bind_knob(m);
    bind_knob_collection(m);
    bind_attribute(m);
    bind_attr_collection(m);
    bind_metadata(m);
    bind_metadata_collection(m);
    bind_node_storage(m);
}
