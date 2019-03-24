#include "computeengine/computeengine.h"

#include <pybind11/pybind11.h>

#include <utility>

using namespace pybind11;
using namespace computeengine;

PYBIND11_MODULE(_computeengine, m)
{

class_<ComputeEngine>(m, "ComputeEngine");

}

