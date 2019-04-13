#include "python/core/bindcore.h"
#include "python/core/pynode.h"

#include "python/foundation/pymap.h"

#include "core/dict.h"
#include "core/node.h"
#include "core/nodestoragetypes.h"

#include <pybind11/pybind11.h>

namespace py = pybind11;

using namespace core;


//
//  PyNode implementation.
//

void PyNode::compute() const
{
    PYBIND11_OVERLOAD_PURE(
        void,
        Node,
        compute);
}

void bind_node(py::handle scope)
{
    PyMap<Node::KnobMap>(scope, "KnobMap");

    py::class_<Node, PyNode, NodePtr>(scope, "Node")
        .def(py::init<Dict>())
        .def("compute", &Node::compute);
}
