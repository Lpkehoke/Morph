#include "platform/node.h"
#include "platform/nodefactory.h"
#include "platform/nodefactoryregistry.h"
#include "platform/nodestorage.h"
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
    class_<Node, boost::noncopyable>("Node", no_init);
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

    static PyObject* convert(float value)
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
    virtual Node* create() const override
    {
        return new DummyNode();
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
        auto factory = new DummyNodeFactory();

        register_node_factory(factory);
    }
};

class NodeStorageAdaptor : public NodeStorage
{
  public:
    NodeStorageAdaptor(NodeFactoryRegistryAdaptor* registry);
    void dispatch(const dict& action);
    void subscribe(const object& on_update);
};


//
//  Python dict conversion utils.
//

template <typename T>
T from_dict(const dict& input);

template <>
Metadata from_dict(const dict& input)
{
    Metadata metadata;
    auto items = input.items();

    for (auto idx = 0; idx < len(items); ++idx)
    {
        object key = items[idx][0];
        std::string key_str = extract<std::string>(key);

        object val = items[idx][1];

        if (PyObject_TypeCheck(val.ptr(), &PyFloat_Type)
            || PyObject_TypeCheck(val.ptr(), &PyLong_Type))
        {
            float val_float = extract<float>(val);
            metadata.set_in_place(key_str, val_float);
        }
        else if (PyObject_TypeCheck(val.ptr(), &PyUnicode_Type))
        {
            std::string val_str = extract<std::string>(val);
            metadata.set_in_place(key_str, val_str);
        }
        else
        {
            throw std::runtime_error("Metadata dictionary only supports float and str types.");
        }
    }

    return metadata;
}

template <>
CreateNode from_dict(const dict& input)
{
    CreateNode create_node;

    create_node.id = extract<NodeId>(input["id"]);
    create_node.model = extract<std::string>(input["model"]);

    dict metadata_dict = extract<dict>(input["metadata"]);
    create_node.metadata = from_dict<Metadata>(metadata_dict);

    return create_node;
}

template <>
RemoveNode from_dict(const dict& input)
{
    RemoveNode remove_node;

    remove_node.id = extract<NodeId>(input["id"]);

    return remove_node;
}

template <>
UpdateNodeMetadata from_dict(const dict& input)
{
    UpdateNodeMetadata update_node_metadata;
    update_node_metadata.id = extract<NodeId>(input["id"]);
    
    dict metadata_dict = extract<dict>(input["metadata"]);
    update_node_metadata.metadata = from_dict<Metadata>(metadata_dict);

    return update_node_metadata;
}

//
//  NodeStorageAdaptor implementation.
//

NodeStorageAdaptor::NodeStorageAdaptor(NodeFactoryRegistryAdaptor* registry)
    : NodeStorage(registry)
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
        NodeStorage::dispatch(update_node_metadata);
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
    class_<NodeStorage::State>("NodeStorageState")
        .add_property(
            "nodes",
            +[](const NodeStorage::State& state)
            {
                return state.m_nodes;
            })
        .add_property(
            "metadata",
            +[](const NodeStorage::State& state)
            {
                return state.m_metadata;
            });

    class_<NodeFactoryRegistryAdaptor>("NodeFactoryRegistry");

    class_<NodeStorageAdaptor>("NodeStorage", init<NodeFactoryRegistryAdaptor*>())
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
