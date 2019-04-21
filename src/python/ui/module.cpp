#include "ui/viewport.h"

#include <pybind11/pybind11.h>

using namespace ui;
namespace py = pybind11;

PYBIND11_MODULE(_ui, m)
{
    py::class_<Viewport>(m, "Viewport")
        .def(py::init<>())
        .def("render", &Viewport::render);
}
