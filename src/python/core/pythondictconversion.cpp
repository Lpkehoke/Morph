#define PYTHON_DICT_CONVERSION_IMPL
#include "core/pythondictconversion.h"

#include "core/dict.h"
#include "core/nodestorageactions.h"
#include "core/nodestoragetypes.h"
#include "core/value/defaultvalues.h"

#include <pybind11/pybind11.h>

#include <stdexcept>
#include <string>
#include <utility>

namespace py = pybind11;

using namespace core;
using namespace foundation;


//
//  Dict converter.
//

template <>
Dict from_python_dict(const py::dict& input)
{
    Dict dict;

    for (const auto& pair : input)
    {
        std::string key_str = pair.first.cast<std::string>();
        py::handle val = pair.second;
        
        if (PyFloat_Check(val.ptr()))
        {
            float val_double = val.cast<float>();
            dict[key_str] << val_double;
        }
        else if (PyUnicode_Check(val.ptr()))
        {
            std::string val_str = val.cast<std::string>();
            dict[key_str] << std::move(val_str);
        }
        else if (PyBool_Check(val.ptr()))
        {
            bool val_bool = val.cast<bool>();
            dict[key_str] << val_bool;
        }
        else if (PyDict_Check(val.ptr()))
        {
            Dict val_dict = from_python_dict<Dict>(val.cast<py::dict>());
            dict[key_str] << std::move(val_dict);
        }
        else
        {
            throw std::runtime_error("Metadata dictionary only supports float, str and bool types.");
        }
    }

    return dict;
}


//
//  CreateNode action converter.
//

template <>
CreateNode from_python_dict(const py::dict& input)
{
    CreateNode create_node;
    create_node.model = input["model"].cast<std::string>();

    py::dict metadata_dict = input["metadata"].cast<py::dict>();
    create_node.metadata = from_python_dict<Dict>(metadata_dict);

    return create_node;
}


//
//  RemoveNode action converter.
//

template <>
RemoveNode from_python_dict(const py::dict& input)
{
    RemoveNode remove_node;

    remove_node.id = (input["id"].cast<NodeId>());

    return remove_node;
}


//
//  UpdateNodeMetadata action converter.
//

template <>
UpdateNodeMetadata from_python_dict(const py::dict& input)
{
    UpdateNodeMetadata update_node_metadata;
    update_node_metadata.id = input["id"].cast<NodeId>();
    
    py::dict metadata_dict = input["metadata"].cast<py::dict>();
    update_node_metadata.metadata = from_python_dict<Dict>(metadata_dict);

    return update_node_metadata;
}


//
//  MakeConnection action converter,
//

template <>
MakeConnection from_python_dict(const py::dict& input)
{
    MakeConnection make_connection;

    make_connection.input_node_id = input["input_node_id"].cast<NodeId>();
    make_connection.input_knob_name = input["input_knob_name"].cast<std::string>();
    make_connection.output_node_id = input["output_node_id"].cast<NodeId>();
    make_connection.output_knob_name = input["output_knob_name"].cast<std::string>();

    return make_connection;
}

