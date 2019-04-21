#include "python/core/bindcore.h"
#include "python/core/pythondictconversion.h"

#include "core/dict.h"
#include "core/value/defaultvalues.h"

#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

#include <cstdint>
#include <memory>
#include <stdexcept>
#include <utility>

using namespace core;

namespace py = pybind11;

namespace
{

void set_item(Dict& self, const std::string& key, const py::object& value)
{
    if (value.ptr() == Py_None)
    {
        self[key] = Value();
    }
    else if (PyDict_Check(value.ptr()))
    {
        self[key] = from_python_dict<Dict>(value.cast<py::dict>());
    }
    else if (PyUnicode_Check(value.ptr()))
    {
        self[key] = value.cast<std::string>();
    }
    else if (PyFloat_Check(value.ptr()))
    {
        self[key] = value.cast<float>();
    }
    else if (PyBool_Check(value.ptr()))
    {
        self[key] = value.cast<bool>();
    }
    else if (PyNumber_Check(value.ptr()))
    {
        self[key] = value.cast<std::int64_t>();
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
        .def(py::init(&from_python_dict<Dict>))
        .def(
            "__getitem__",
            [](const Dict& self, const std::string& key)
            {
                return self[key];
            })
        .def("__setitem__", &set_item);
}
