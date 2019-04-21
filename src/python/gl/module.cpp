#include "gl/gl.h"

#include <pybind11/pybind11.h>

using namespace gl;
namespace py = pybind11;

PYBIND11_MODULE(_gl, m)
{
    m.def("init_opengl", &init_opengl);
}
