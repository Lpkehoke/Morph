#pragma once

#include "core/nodestoragetypes.h"

#include "foundation/immutable/map.h"

namespace computeengine { class ComputeTask; }

namespace computeengine
{

using DepsGraph = foundation::Map<core::NodeId, ComputeTask*>;

} // namespace computeengine
