#pragma once

#include <pybind11/pybind11.h>

template <typename Map>
class __attribute__ ((visibility("hidden"))) PyMap : public pybind11::class_<Map>
{
  public:
    using pybind11::class_<Map>::def;

    PyMap(pybind11::handle scope, const char *name)
      : pybind11::class_<Map>(scope, name)
    {
        def(pybind11::init<>());
        def("__len__", &Map::size);
        def(
            "__iter__",
            [](const Map &obj) 
            {
                return pybind11::make_iterator(obj.begin(), obj.end());
            },
            pybind11::keep_alive<0, 1>());
        def("__getitem__", &Map::get);
    }
};
