#include "computeengine/computeengine.h"

#include <pybind11/pybind11.h>

#include <utility>

namespace py = pybind11;

using namespace computeengine;

PYBIND11_MODULE(_computeengine, m)
{

py::class_<ComputeEngine>(m, "ComputeEngine");

}

