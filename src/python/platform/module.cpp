#include "pythondictconversion.inl"

#include "python/base/bindimmutablemap.h"

#include "platform/knobmodelregistry.h"
#include "platform/logger.h"
#include "platform/node.h"
#include "platform/nodefactory.h"
#include "platform/nodefactoryregistry.h"
#include "platform/nodestorage.h"
#include "platform/nodestorageactions.h"
#include "platform/nodestoragetypes.h"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <exception>

using namespace platform;
using namespace pybind11;


//
//  Bind Node.
//

void bind_node(const handle& m)
{
    bind_map<Node::KnobMap>(m, "KnobMap");

    class_<Node, NodePtr>(m, "Node")
        .def("input_knobs", &Node::input_knobs)
        .def("output_knobs", &Node::output_knobs);
}

void bind_node_collection(const handle& m)
{
    bind_map<NodeCollection>(m, "NodeCollection");
}


//
//  Bind Knob.
//

void bind_knob(const handle& m)
{
    class_<Knob, KnobPtr>(m, "Knob");
}

void bind_knob_collection(const handle& m)
{
    bind_map<KnobCollection>(m, "KnobKollection");
}


//
//  Bind Metadata.
//

void bind_metadata(const handle& m)
{
    bind_map<Metadata>(m, "Metadata");
}

void bind_metadata_collection(const handle& m)
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
    virtual Node* create(const KnobsMap& knobs) const override
    {
        return new DummyNode();
    }

    virtual std::string model() const override
    {
        return "Dummy";
    }

    virtual KnobInitInfoMap knobs_init_info() const override
    {
        return KnobInitInfoMap();
    }
};

class NodeFactoryRegistryAdaptor : public NodeFactoryRegistry
{
  public:
    NodeFactoryRegistryAdaptor()
    {
        auto factory = new DummyNodeFactory();

        register_node_factory(factory);
    }
};

class NodeStorageAdaptor : public NodeStorage
{
  public:
    NodeStorageAdaptor(
        NodeFactoryRegistryAdaptor* registry,
        KnobModelRegistry*         builder,
        Logger*                     logger);

    void dispatch(const dict& action);
    void subscribe(const object& on_update);
};


//
//  NodeStorageAdaptor implementation.
//

NodeStorageAdaptor::NodeStorageAdaptor(
    NodeFactoryRegistryAdaptor* registry,
    KnobModelRegistry*         builder,
    Logger*                     logger)
    : NodeStorage(registry, builder, logger)
{}

void NodeStorageAdaptor::dispatch(const dict& action)
{
    std::string action_type = str(action["type"]);

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

void NodeStorageAdaptor::subscribe(const object& on_update)
{
    NodeStorage::subscribe(
        [on_update]()
        {
            gil_scoped_acquire gil;

            try
            {
                on_update();
            }
            catch(const error_already_set&)
            {
                // TODO: supply exception info here.
                throw std::runtime_error("Python exception occured during callback evaluation.");
            }
        });
}

} // namespace

void bind_node_storage(const handle& m)
{
    class_<NodeStorageState>(m, "NodeStorageState")
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

    class_<NodeFactoryRegistryAdaptor>(m, "NodeFactoryRegistry")
        .def(init<>());
    class_<KnobModelRegistry>(m, "KnobModelRegistry")
        .def(init<>());
    class_<Logger>(m, "Logger")
        .def(init<>());

    class_<NodeStorageAdaptor>(m, "NodeStorage")
        .def(init<NodeFactoryRegistryAdaptor*, KnobModelRegistry*, Logger*>())
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
    bind_metadata(m);
    bind_metadata_collection(m);
    bind_node_storage(m);
}
