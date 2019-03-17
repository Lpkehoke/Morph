#pragma once

#include "platform/nodestorageactions.h"
#include "platform/nodestoragetypes.h"

#include <boost/python.hpp>

#include <exception>

using namespace platform;
using namespace boost::python;

template <typename T>
T from_dict(const boost::python::dict& input);


//
//  Metadata converter.
//

template <>
Metadata from_dict(const dict& input)
{
    Metadata metadata;
    auto items = input.items();
    auto items_size = len(items);

    for (auto idx = 0; idx < items_size; ++idx)
    {
        object key = items[idx][0];
        std::string key_str = extract<std::string>(key);

        object val = items[idx][1];

        if (PyObject_TypeCheck(val.ptr(), &PyFloat_Type)
            || PyObject_TypeCheck(val.ptr(), &PyLong_Type))
        {
            double val_double = PyFloat_AsDouble(val.ptr());
            metadata.mutable_set(key_str, val_double);
        }
        else if (PyObject_TypeCheck(val.ptr(), &PyUnicode_Type))
        {
            std::string val_str = extract<std::string>(val);
            metadata.mutable_set(key_str, val_str);
        }
        else
        {
            throw std::runtime_error("Metadata dictionary only supports float and str types.");
        }
    }

    return metadata;
}


//
//  CreateNode action converter.
//

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


//
//  RemoveNode action converter.
//

template <>
RemoveNode from_dict(const dict& input)
{
    RemoveNode remove_node;

    remove_node.id = extract<NodeId>(input["id"]);

    return remove_node;
}


//
//  UpdateNodeMetadata action converter.
//

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
//  MakeConnection action converter,
//

template <>
MakeConnection from_dict(const dict& input)
{
    MakeConnection make_connection;

    make_connection.input_node_id = extract<NodeId>(input["input_node_id"]);
    make_connection.input_knob_name = extract<std::string>(input["input_knob_name"]);
    make_connection.output_node_id = extract<NodeId>(input["output_node_id"]);
    make_connection.output_knob_name = extract<std::string>(input["output_knob_name"]);

    return make_connection;
}

