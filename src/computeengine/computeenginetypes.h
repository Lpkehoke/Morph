#pragma once

#include "platform/nodestoragetypes.h"

#include "base/immutable/map.h"

namespace computeengine { class ComputeTask; }

namespace computeengine
{

using DepsGraph = immutable::Map<platform::NodeId, ComputeTask*>;

} // namespace computeengine
