#pragma once

#include <pybind11/pybind11.h>


template <typename Map>
auto bind_map(const pybind11::handle& scope, const char* name)
{
    return pybind11::class_<Map>(scope, name)
        .def("__len__", &Map::size)
        .def(
            "__iter__",
            [](const Map &obj) 
            {
                return pybind11::make_iterator(obj.begin(), obj.end());
            },
            pybind11::keep_alive<0, 1>())
        .def("__getitem__", &Map::get);
}
