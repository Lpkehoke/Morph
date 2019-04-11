#include "python/platform/bindplatform.h"
#include "python/platform/pynode.h"

#include "python/base/bindimmutablemap.h"

#include "platform/node.h"
#include "platform/nodestoragetypes.h"

#include <pybind11/pybind11.h>

namespace py = pybind11;

using namespace platform;


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
    ImmutableMap<Node::KnobMap>(scope, "KnobMap");

    py::class_<Node, PyNode, NodePtr>(scope, "Node")
        .def("compute", &Node::compute);
}
