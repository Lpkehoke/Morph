#pragma once

#include "platform/nodestorageactions.h"
#include "platform/nodestoragetypes.h"

#include <pybind11/pybind11.h>

#include <exception>

using namespace platform;
using namespace pybind11;

template <typename T>
T from_dict(const dict& input);


//
//  Metadata converter.
//

template <>
Metadata from_dict(const dict& input)
{
    Metadata metadata;

    for (const auto& pair : input)
    {
        std::string key_str = pair.first.cast<std::string>();
        handle val = pair.second;

        if (PyObject_TypeCheck(val.ptr(), &PyFloat_Type)
            || PyObject_TypeCheck(val.ptr(), &PyLong_Type))
        {
            double val_double = val.cast<double>();
            metadata.mutable_set(key_str, val_double);
        }
        else if (PyObject_TypeCheck(val.ptr(), &PyUnicode_Type))
        {
            std::string val_str = val.cast<std::string>();
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

    create_node.id = input["id"].cast<NodeId>();
    create_node.model = input["model"].cast<std::string>();

    dict metadata_dict = input["metadata"].cast<dict>();
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

    remove_node.id = (input["id"].cast<NodeId>());

    return remove_node;
}


//
//  UpdateNodeMetadata action converter.
//

template <>
UpdateNodeMetadata from_dict(const dict& input)
{
    UpdateNodeMetadata update_node_metadata;
    update_node_metadata.id = input["id"].cast<NodeId>();
    
    dict metadata_dict = input["metadata"].cast<dict>();
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

    make_connection.input_node_id = input["input_node_id"].cast<NodeId>();
    make_connection.input_knob_name = input["input_knob_name"].cast<std::string>();
    make_connection.output_node_id = input["output_node_id"].cast<NodeId>();
    make_connection.output_knob_name = input["output_knob_name"].cast<std::string>();

    return make_connection;
}

