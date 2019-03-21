#include "pythondictconversion.inl"

#include "platform/knobmodelregistry.h"
#include "platform/logger.h"
#include "platform/node.h"
#include "platform/nodefactory.h"
#include "platform/nodefactoryregistry.h"
#include "platform/nodestorage.h"
#include "platform/nodestorageactions.h"
#include "platform/nodestoragetypes.h"

#include <boost/python.hpp>

#include <exception>

using namespace boost::python;
using namespace platform;


//
//  Bind Node.
//

void bind_node()
{
    class_<Node::KnobMap>("KnobMap")
        .def(
            "__getitem__",
            &Node::KnobMap::get,
            return_value_policy<copy_const_reference>())
        .def(
            "__iter__",
            range(&Node::KnobMap::begin, &Node::KnobMap::end));

    class_<Node, NodePtr>("Node", no_init)
        .def("input_knobs", &Node::input_knobs)
        .def("output_knobs", &Node::output_knobs);
}

void bind_node_collection()
{
    class_<NodeCollection>("NodeCollection")
        .def(
            "__getitem__",
            &NodeCollection::get,
            return_internal_reference<>())
        .def(
            "__iter__",
            range(&NodeCollection::begin, &NodeCollection::end));
}


//
//  Bind Knob.
//

void bind_knob()
{
    class_<Knob, KnobPtr>("Knob", no_init)
        .def(
            "__getitem__",
            &Knob::attribute)
        .def(
            "__iter__",
            range(&Knob::begin, &Knob::end));
}

void bind_knob_collection()
{
    class_<KnobCollection>("KnobKollection")
        .def(
            "__getitem__",
            &KnobCollection::get,
            return_internal_reference<>())
        .def(
            "__iter__",
            range(&KnobCollection::begin, &KnobCollection::end));
}


//
//  Bind Metadata.
//

namespace
{

struct MetadataPropertyConverter
{
    static PyObject* convert(const MetadataProperty& property)
    {
        return std::visit(
            [](const auto& value)
            {
                return convert(value); 
            }, property);
    }

    static PyObject* convert(const std::string& value)
    {
        return PyUnicode_FromString(value.c_str());
    }

    static PyObject* convert(double value)
    {
        return PyFloat_FromDouble(value);
    }
};

struct MetadataPairConverter
{
    static PyObject* convert(const Metadata::Pair& pair)
    {
        auto key = PyUnicode_FromString(pair.first.c_str());
        auto val = MetadataPropertyConverter::convert(pair.second);

        auto tuple = PyTuple_Pack(2, key, val);

        return tuple;
    }
};

} // namespace

void bind_metadata()
{
    to_python_converter<MetadataProperty, MetadataPropertyConverter>();
    to_python_converter<Metadata::Pair, MetadataPairConverter>();

    class_<Metadata>("Metadata")
        .def(
            "__getitem__",
            &Metadata::get,
            boost::python::return_value_policy<copy_const_reference>())
        .def(
            "__iter__",
            range(&Metadata::begin, &Metadata::end));
}

void bind_metadata_collection()
{
    class_<MetadataCollection>("MetadataCollection")
        .def(
            "__getitem__",
            &MetadataCollection::get,
            boost::python::return_value_policy<copy_const_reference>());
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
    std::string action_type = extract<std::string>(action["type"]);

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
            auto state = PyGILState_Ensure();

            try
            {
                on_update();
            }
            catch(const error_already_set&)
            {
                PyGILState_Release(state);
                // TODO: supply exception info here.
                throw std::runtime_error("Python exception occured during callback evaluation.");
            }
            
            PyGILState_Release(state);
        });
}

} // namespace

void bind_node_storage()
{
    class_<NodeStorageState>("NodeStorageState")
        .add_property(
            "nodes",
            +[](const NodeStorageState& state)
            {
                return state.m_nodes;
            })
        .add_property(
            "knobs",
            +[](const NodeStorageState& state)
            {
                return state.m_knobs;
            })
        .add_property(
            "attributes",
            +[](const NodeStorageState& state)
            {
                return state.m_attributes;
            })
        .add_property(
            "node_metadata",
            +[](const NodeStorageState& state)
            {
                return state.m_node_metadata;
            });

    class_<NodeFactoryRegistryAdaptor>("NodeFactoryRegistry");
    class_<KnobModelRegistry>("KnobModelRegistry");
    class_<Logger>("Logger");

    class_<NodeStorageAdaptor>(
        "NodeStorage",
        init<NodeFactoryRegistryAdaptor*, KnobModelRegistry*, Logger*>())
        .def("dispatch", &NodeStorageAdaptor::dispatch)
        .def("state", &NodeStorageAdaptor::state)
        .def("subscribe", &NodeStorageAdaptor::subscribe);

}


//
//  Bind platform.
//

BOOST_PYTHON_MODULE(_platform)
{
    bind_node();
    bind_node_collection();
    bind_metadata();
    bind_metadata_collection();
    bind_node_storage();
}
