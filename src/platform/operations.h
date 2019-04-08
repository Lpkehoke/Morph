#pragma once

#include "platform/nodestoragetypes.h"

namespace platform { class KnobSchema; }
namespace platform { class NodeFactory; }

namespace platform
{

NodeStorageState make_knob(NodeStorageState state, NodeId owner, const KnobSchema& knob_schema);

NodeStorageState make_node(NodeStorageState state, const NodeFactory& node_factory);

} // namespace platform
