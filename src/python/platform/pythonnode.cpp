#include "pythonnode.h"

using namespace platform;
using namespace boost::python;

namespace python
{

PythonNode::PythonNode(const object& py_object)
    : m_py_object(py_object)
{}

void PythonNode::compute() const
{
    m_py_object.attr("compute")();
}

} // namespace python
