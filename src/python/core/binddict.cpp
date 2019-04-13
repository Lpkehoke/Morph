#include "python/core/bindcore.h"
#include "python/core/pythondictconversion.h"

#include "core/dict.h"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <cstdint>
#include <memory>
#include <stdexcept>
#include <utility>
#include <variant>

using namespace core;

namespace py = pybind11;

namespace
{

void set_item(Dict& self, const std::string& key, const py::object& value)
{
    if (value.ptr() == Py_None)
    {
        self[key] = std::monostate{};
    }
    else if (PyDict_Check(value.ptr()))
    {
        self[key] = from_dict<Dict>(value.cast<py::dict>());
    }
    else if (PyUnicode_Check(value.ptr()))
    {
        self[key] = value.cast<std::string>();
    }
    else if (PyFloat_Check(value.ptr()))
    {
        self[key] = value.cast<double>();
    }
    else if (PyBool_Check(value.ptr()))
    {
        self[key] = value.cast<bool>();
    }
    else if (PyNumber_Check(value.ptr()))
    {
        self[key] = value.cast<std::uint64_t>();
    }
    else
    {
        throw std::runtime_error("Unsupported value type.");
    }
}

} // namespace

void bind_dict(py::handle scope)
{
    py::class_<Dict>(scope, "Dict")
        .def(py::init<>())
        .def(py::init(&from_dict<Dict>))
        .def(
            "__getitem__",
            [](const Dict& self, const std::string& key)
            {
                return self[key];
            })
        .def("__setitem__", &set_item)
        .def("__repr__", &Dict::to_string);
}
