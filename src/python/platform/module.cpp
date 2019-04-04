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
#include <pybind11/stl.h>

#include <exception>
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

// Dummy node is used for testing until NodeStorageRegistry is
// populated with nodes from plugins.
class DummyNode : public Node
{
  public:
    DummyNode() = default;

    virtual void compute() const override
    {
        return;
    }
};

// Dummy node factory is used for testing until NodeStorageRegistry is
// populated with nodes from plugins.
class DummyNodeFactory : public NodeFactory
{
  public:
    virtual NodeStorageState create(NodeStorageState state) const override
    {
        auto node = std::make_shared<DummyNode>();
        auto next_state = std::move(state);
        next_state.m_nodes.mutable_set(next_state.m_next_node_id++, std::move(node));

        return next_state;
    }

    virtual std::string model() const override
    {
        return "Dummy";
    }
};

class NodeFactoryRegistryAdaptor : public NodeFactoryRegistry
{
  public:
    NodeFactoryRegistryAdaptor()
    {
        auto node_factory = new DummyNodeFactory();

        register_node_factory(node_factory);
    }
};

class NodeStorageAdaptor : public NodeStorage
{
  public:
    NodeStorageAdaptor(
        NodeFactoryRegistryAdaptor* node_factory_registry,
        Logger*                     logger);

    void dispatch(const py::dict& action);
    void subscribe(const py::object& on_update);
};


//
//  NodeStorageAdaptor implementation.
//

NodeStorageAdaptor::NodeStorageAdaptor(
    NodeFactoryRegistryAdaptor* node_factory_registry,
    Logger*                     logger)
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

    py::class_<NodeFactoryRegistryAdaptor>(m, "NodeFactoryRegistry")
        .def(py::init<>());
    py::class_<Logger>(m, "Logger")
        .def(py::init<>());

    py::class_<NodeStorageAdaptor>(m, "NodeStorage")
        .def(py::init<NodeFactoryRegistryAdaptor*, Logger*>())
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
