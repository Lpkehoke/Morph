#define PYTHON_DICT_CONVERSION_IMPL
#include "core/pythondictconversion.h"

#include "core/nodestorageactions.h"
#include "core/nodestoragetypes.h"

#include <pybind11/pybind11.h>

#include <exception>

namespace py = pybind11;

using namespace core;


//
//  Metadata converter.
//

template <>
Metadata from_dict(const py::dict& input)
{
    Metadata metadata;

    for (const auto& pair : input)
    {
        std::string key_str = pair.first.cast<std::string>();
        py::handle val = pair.second;
        
        if (PyFloat_Check(val.ptr()))
        {
            double val_double = val.cast<double>();
            metadata.mutable_set(key_str, val_double);
        }
        else if (PyUnicode_Check(val.ptr()))
        {
            std::string val_str = val.cast<std::string>();
            metadata.mutable_set(key_str, val_str);
        }
        else if (PyBool_Check(val.ptr()))
        {
            bool val_bool = val.cast<bool>();
            metadata.mutable_set(key_str, val_bool);
        }
        else
        {
            throw std::runtime_error("Metadata dictionary only supports float, str and bool types.");
        }
    }

    return metadata;
}


//
//  CreateNode action converter.
//

template <>
CreateNode from_dict(const py::dict& input)
{
    CreateNode create_node;
    create_node.model = input["model"].cast<std::string>();

    py::dict metadata_dict = input["metadata"].cast<py::dict>();
    create_node.metadata = from_dict<Metadata>(metadata_dict);

    return create_node;
}


//
//  RemoveNode action converter.
//

template <>
RemoveNode from_dict(const py::dict& input)
{
    RemoveNode remove_node;

    remove_node.id = (input["id"].cast<NodeId>());

    return remove_node;
}


//
//  UpdateNodeMetadata action converter.
//

template <>
UpdateNodeMetadata from_dict(const py::dict& input)
{
    UpdateNodeMetadata update_node_metadata;
    update_node_metadata.id = input["id"].cast<NodeId>();
    
    py::dict metadata_dict = input["metadata"].cast<py::dict>();
    update_node_metadata.metadata = from_dict<Metadata>(metadata_dict);

    return update_node_metadata;
}


//
//  MakeConnection action converter,
//

template <>
MakeConnection from_dict(const py::dict& input)
{
    MakeConnection make_connection;

    make_connection.input_node_id = input["input_node_id"].cast<NodeId>();
    make_connection.input_knob_name = input["input_knob_name"].cast<std::string>();
    make_connection.output_node_id = input["output_node_id"].cast<NodeId>();
    make_connection.output_knob_name = input["output_knob_name"].cast<std::string>();

    return make_connection;
}

